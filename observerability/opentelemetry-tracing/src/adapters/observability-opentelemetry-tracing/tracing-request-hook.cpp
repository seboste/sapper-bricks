#include "tracing-request-hook.h"
#include <adapters/observability-opentelemetry-tracing/impl/mse-context-carrier.h>

#include <opentelemetry/trace/provider.h>
#include <opentelemetry/trace/tracer.h>
#include <opentelemetry/context/propagation/text_map_propagator.h>

#include <opentelemetry/trace/propagation/b3_propagator.h>
#include <opentelemetry/trace/propagation/http_trace_context.h>
#include <opentelemetry/trace/context.h>

#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"

#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter_options.h"

using namespace opentelemetry;
using namespace opentelemetry::trace;

namespace {
 
nostd::shared_ptr<Tracer> get_app_tracer()
{
    auto provider = opentelemetry::trace::Provider::GetTracerProvider(); 
    const mse::Context& global_context = mse::Context::GetGlobalContext();
    return provider->GetTracer(
        global_context.AtOr("app","UNKNOWN"), 
        global_context.AtOr("version","UNKNOWN")
        );
}

nostd::shared_ptr<context::propagation::TextMapPropagator> create_propagator(TracingRequestHook::Parameters::PropagationType propagation_type)
{
    using PropagationType = TracingRequestHook::Parameters::PropagationType;
    using namespace propagation;

    switch (propagation_type)
    {
    case PropagationType::B3_SingleHeader:
        return nostd::shared_ptr<B3Propagator>(new B3Propagator());
        break;
    case PropagationType::B3_MultiHeader:
        return nostd::shared_ptr<B3PropagatorMultiHeader>(new B3PropagatorMultiHeader());
        break;
    case PropagationType::Http:
        return nostd::shared_ptr<HttpTraceContext>(new HttpTraceContext());
        break;
    default:
        break;
    }
    return nostd::shared_ptr<context::propagation::TextMapPropagator>(nullptr);
}

} //anon namespace

const std::vector<std::string>& TracingRequestHook::Parameters::get_propagation_headers() const
{
    using namespace opentelemetry::trace::propagation;
    static std::unordered_map<PropagationType, std::vector<std::string> > propagation_headers =
    {
        { PropagationType::B3_SingleHeader, std::vector<std::string>{ std::string(kB3CombinedHeader) }},
        { PropagationType::B3_MultiHeader, std::vector<std::string>{ std::string(kB3TraceIdHeader), std::string(kB3SpanIdHeader), std::string(kB3SampledHeader) }},
        { PropagationType::Http, std::vector<std::string>{ std::string(kTraceParent), std::string(kTraceState) }}        
    };
    return propagation_headers[propagation_type];
}
    
TracingRequestHook::TracerProvider::TracerProvider(const Parameters& parameters)
{
    const mse::Context& global_context = mse::Context::GetGlobalContext();
    opentelemetry::exporter::otlp::OtlpHttpExporterOptions opts;
    opts.url = parameters.otel_url;
    auto exporter = opentelemetry::exporter::otlp::OtlpHttpExporterFactory::Create(opts);
    auto processor = opentelemetry::sdk::trace::SimpleSpanProcessorFactory::Create(std::move(exporter));
    auto resource = opentelemetry::sdk::resource::Resource::Create({
        {"service.name", global_context.AtOr("app","UNKNOWN")}, 
        {"service.version", global_context.AtOr("version","UNKNOWN")}
    });
    std::shared_ptr<opentelemetry::trace::TracerProvider> provider = opentelemetry::sdk::trace::TracerProviderFactory::Create(std::move(processor), resource);
    opentelemetry::trace::Provider::SetTracerProvider(provider);
}

TracingRequestHook::TracerProvider::~TracerProvider()
{
    std::shared_ptr<opentelemetry::trace::TracerProvider> none;
    opentelemetry::trace::Provider::SetTracerProvider(none);
}

struct TracingRequestHook::Impl
{
    Parameters parameters;
    nostd::shared_ptr<context::propagation::TextMapPropagator> propagator;
    nostd::shared_ptr<Tracer> tracer;
    std::unique_ptr<Scope> remote_scope;
    std::unique_ptr<Scope> scope;
    nostd::shared_ptr<Span> span;
};

TracingRequestHook::TracingRequestHook(const Parameters& parameters)
    : mse::RequestHook("tracing"), _pimpl(std::make_unique<TracingRequestHook::Impl>(TracingRequestHook::Impl{ 
        parameters, create_propagator(parameters.propagation_type), get_app_tracer() }))
{
}
 
TracingRequestHook::~TracingRequestHook()
{
}

mse::Status TracingRequestHook::pre_process(mse::Context& context)
{
    //1. create remote parent span for incoming requests from the request metadata
    if(GetRequestType() == mse::RequestType::incoming)
    {
        const MSEContextCarrier carrier(context);
        auto current_ctx = context::RuntimeContext::GetCurrent();
        auto new_ctx = _pimpl->propagator->Extract(carrier, current_ctx);
        auto remote_span = opentelemetry::trace::GetSpan(new_ctx);
        _pimpl->remote_scope = std::make_unique<Scope>(_pimpl->tracer->WithActiveSpan(remote_span));        
    }

    //2. create a new span for the current request
    _pimpl->span = _pimpl->tracer->StartSpan(context.AtOr("request", "UNKNOWN"));    
    _pimpl->scope = std::make_unique<Scope>(_pimpl->tracer->WithActiveSpan(_pimpl->span));
    
    //3. make sure that the request metadata now contains the information of the current span
    MSEContextCarrier carrier(mse::Context::GetThreadLocalContext());            
    _pimpl->propagator->Inject(carrier, context::RuntimeContext::GetCurrent());
    
    return mse::Status::OK;
}

mse::Status TracingRequestHook::post_process(mse::Context& context, mse::Status status)
{
    //1. end the current span with the request status
    _pimpl->span->SetStatus(status ? StatusCode::kOk : StatusCode::kError, status.details);
    _pimpl->span->End();
    _pimpl->scope = nullptr;
    _pimpl->span = nullptr;

    //2. reset the request metadata to the remote span and finalize remote span
    MSEContextCarrier carrier(mse::Context::GetThreadLocalContext());
    _pimpl->propagator->Inject(carrier, context::RuntimeContext::GetCurrent());
    _pimpl->remote_scope = nullptr;

    return status;
}

#pragma once

#include <microservice-essentials/request/request-hook-factory.h>
#include <microservice-essentials/request/request-hook.h>

class TracingRequestHook : public mse::RequestHook
{
public:    
    struct Parameters
    {
        enum class PropagationType
        {
            B3_SingleHeader,    // b3: <TraceId>-<SpanId>-1-<ParentSpanID> e.g. 80f198ee56343ba864fe8b2a57d3eff7-e457b5a2e4d86bd1-1-05e3ac9a4f6e3b90
            B3_MultiHeader,     // X-B3-TraceId: <TraceId>; X-B3-SpanId: <SpanId>; X-B3-ParentSpanId: <ParentSpanID>
            Http                // traceparent; tracestate            
        };

        const std::vector<std::string>& get_propagation_headers() const;

        std::string otel_url = "http://localhost:4318/v1/traces";
        PropagationType propagation_type = PropagationType::B3_SingleHeader;
        mse::AutoRequestHookParameterRegistration<TracingRequestHook::Parameters, TracingRequestHook> auto_registration;
    };

    class TracerProvider
    {
    public:
        TracerProvider(const Parameters& parameters);
        virtual ~TracerProvider();
    };

    TracingRequestHook(const Parameters& parameters);
    virtual ~TracingRequestHook();    

    virtual mse::Status pre_process(mse::Context& context) override;
    virtual mse::Status post_process(mse::Context& context, mse::Status status) override;
private:

    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};

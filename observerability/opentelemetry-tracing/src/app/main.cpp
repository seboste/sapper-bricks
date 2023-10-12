//<<<SAPPER SECTION BEGIN MERGE MAIN-INCLUDES>>>
#include <adapters/observability-opentelemetry-tracing/tracing-request-hook.h>
//<<<SAPPER SECTION END MERGE MAIN-INCLUDES>>>

int main(int argc, char **argv)
{
    try
    {
//<<<SAPPER SECTION BEGIN APPEND MAIN-INITIALIZATION>>>
        TracingRequestHook::Parameters tracer_parameters{
            mse::getenv_or("OPENTELEMETRY_URL", "http://<<<OPENTELEMETRY_HOST>>>:<<<OPENTELEMETRY_PORT>>>/v1/traces"),
            TracingRequestHook::Parameters::PropagationType::B3_SingleHeader
        };
        TracingRequestHook::TracerProvider tracer_provider(tracer_parameters);
//<<<SAPPER SECTION END APPEND MAIN-INITIALIZATION>>>

        std::vector<std::string> headers_to_propagate;

//<<<SAPPER SECTION BEGIN PREPEND MAIN-REQUEST-HANDLER-HOOKS>>>
        headers_to_propagate.insert(headers_to_propagate.end(),
                tracer_parameters.get_propagation_headers().begin(), tracer_parameters.get_propagation_headers().end()
        );
        mse::RequestHandler::GloballyWith(tracer_parameters);
//<<<SAPPER SECTION END PREPEND MAIN-REQUEST-HANDLER-HOOKS>>>

//<<<SAPPER SECTION BEGIN PREPEND MAIN-REQUEST-ISSUER-HOOKS>>>
        mse::RequestIssuer::GloballyWith(tracer_parameters);
//<<<SAPPER SECTION END PREPEND MAIN-REQUEST-ISSUER-HOOKS>>>

//<<<SAPPER SECTION BEGIN APPEND MAIN-CLEANUP>>>
//<<<SAPPER SECTION END APPEND MAIN-CLEANUP>>>
    }
}


//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/observability/logging-request-hook.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
    ...

//<<<SAPPER SECTION BEGIN PREPEND MAIN-INITIALIZATION>>>
    mse::ConsoleLogger logger;
    mse::StructuredLogger structured_logger(logger, { <<<STRUCTURED_LOG_FIELDS>>> } );
//<<<SAPPER SECTION END PREPEND MAIN-INITIALIZATION>>>

//<<<SAPPER SECTION BEGIN PREPEND MAIN-REQUEST-HANDLER-HOOKS>>>
    mse::RequestHandler::GloballyWith(mse::LoggingRequestHook::Parameters{});
//<<<SAPPER SECTION END PREPEND MAIN-REQUEST-HANDLER-HOOKS>>>

//<<<SAPPER SECTION BEGIN PREPEND MAIN-REQUEST-ISSUER-HOOKS>>>
    mse::RequestIssuer::GloballyWith(mse::LoggingRequestHook::Parameters{});
//<<<SAPPER SECTION END PREPEND MAIN-REQUEST-ISSUER-HOOKS>>>

    ...
}

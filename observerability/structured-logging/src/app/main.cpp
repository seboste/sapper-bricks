//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/observability/logging-request-hook.h>
#include <exception>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
    try
    {
//<<<SAPPER SECTION BEGIN PREPEND MAIN-INITIALIZATION>>>
        static mse::ConsoleLogger logger; //make sure that the logger lifetime exceeds the try/catch block
        static mse::StructuredLogger structured_logger(logger, <<<STRUCTURED_LOG_FIELDS>>> );
//<<<SAPPER SECTION END PREPEND MAIN-INITIALIZATION>>>

//<<<SAPPER SECTION BEGIN PREPEND MAIN-REQUEST-HANDLER-HOOKS>>>
        mse::RequestHandler::GloballyWith(mse::LoggingRequestHook::Parameters{});
//<<<SAPPER SECTION END PREPEND MAIN-REQUEST-HANDLER-HOOKS>>>

//<<<SAPPER SECTION BEGIN PREPEND MAIN-REQUEST-ISSUER-HOOKS>>>
        mse::RequestIssuer::GloballyWith(mse::LoggingRequestHook::Parameters{});
//<<<SAPPER SECTION END PREPEND MAIN-REQUEST-ISSUER-HOOKS>>>
        
        ...

    }
    catch(const std::exception& e)
    {
//<<<SAPPER SECTION BEGIN REPLACE MAIN-STD-EXCEPTION-HANDLER>>>
        MSE_LOG_CRITICAL(std::string("received exception: ") + e.what() + ". Terminating.");
//<<<SAPPER SECTION END REPLACE MAIN-STD-EXCEPTION-HANDLER>>>
        return 1;
    }
    catch(...)
    {
//<<<SAPPER SECTION BEGIN REPLACE MAIN-UNKNOWN-EXCEPTION-HANDLER>>>
        MSE_LOG_CRITICAL("received unknown exception. Terminating.");
//<<<SAPPER SECTION END REPLACE MAIN-UNKNOWN-EXCEPTION-HANDLER>>>
        return 1;
    }    
    return 0;
}

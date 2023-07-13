//<<<SAPPER SECTION BEGIN MAIN-INCLUDES>>>
#include <core/core.h>
#include <memory>
#include <microservice-essentials/context.h>
#include <microservice-essentials/handler.h>
#include <microservice-essentials/utilities/environment.h>
#include <microservice-essentials/cross-cutting-concerns/exception-handling-request-hook.h>
#include <microservice-essentials/cross-cutting-concerns/graceful-shutdown.h>
#include <microservice-essentials/request/request-processor.h>
//<<<SAPPER SECTION END MAIN-INCLUDES>>>

int main(int argc, char **argv)
{
    mse::Context::GetGlobalContext().Insert({   
//<<<SAPPER SECTION BEGIN MAIN-ENVIRONMENT>>>
//<<<SAPPER SECTION END MAIN-ENVIRONMENT>>>
        {"app", mse::getenv_or("APP", "<<<NAME>>>") },
        {"version", mse::getenv_or("VERSION", "<<<VERSION>>>") }
    });

    try
    {
//<<<SAPPER SECTION BEGIN MAIN-INITIALIZATION>>>
//<<<SAPPER SECTION END MAIN-INITIALIZATION>>>

        const std::vector<std::string> headers_to_propagate = {
//<<<SAPPER SECTION BEGIN MAIN-HEADERS-TO-PROPAGATE>>> //all entries must be lower case
//<<<SAPPER SECTION END MAIN-HEADERS-TO-PROPAGATE>>>
                "" //shouldn't do anything but avoids compiler errors when strings are added with a ,-suffix
        };

//<<<SAPPER SECTION BEGIN MAIN-REQUEST-HANDLER-HOOKS>>>
        mse::RequestHandler::GloballyWith(mse::ExceptionHandlingRequestHook::Parameters{});
//<<<SAPPER SECTION END MAIN-REQUEST-HANDLER-HOOKS>>>

//<<<SAPPER SECTION BEGIN MAIN-REQUEST-ISSUER-HOOKS>>>
//<<<SAPPER SECTION END MAIN-REQUEST-ISSUER-HOOKS>>>
    
//<<<SAPPER SECTION BEGIN MAIN-REPO-INSTANTIATION>>>
        std::unique_ptr<EntityRepo> repo = nullptr;
//<<<SAPPER SECTION END MAIN-REPO-INSTANTIATION>>>

//<<<SAPPER SECTION BEGIN MAIN-NOTIFIER-INSTANTIATION>>>
        std::unique_ptr<EntityNotifier> notifier = nullptr;
//<<<SAPPER SECTION END MAIN-NOTIFIER-INSTANTIATION>>>

        Core core(repo.get(), notifier.get());

//<<<SAPPER SECTION BEGIN MAIN-HANDLER-INSTANTIATION>>>    
        class DummyHandler : public mse::Handler
        {
        public:
            DummyHandler() : Handler("dummy-handler") {};
            ~DummyHandler() = default;    

            virtual void Handle() {}
            virtual void Stop() {};
        };
        DummyHandler handler;
//<<<SAPPER SECTION END MAIN-HANDLER-INSTANTIATION>>>

        mse::GracefulShutdownOnSignal gracefulShutdown(mse::Signal::SIG_SHUTDOWN);

        handler.Handle();
    }
    catch(const std::exception& e)
    {
//<<<SAPPER SECTION BEGIN MAIN-STD-EXCEPTION-HANDLER>>>
        std::cerr << "received exception: " << e.what() << ". Terminating." << std::endl;
//<<<SAPPER SECTION END MAIN-STD-EXCEPTION-HANDLER>>>
        return 1;
    }
    catch(...)
    {
//<<<SAPPER SECTION BEGIN MAIN-UNKNOWN-EXCEPTION-HANDLER>>>
        std::cerr << "received unknown exception. Terminating." << std::endl;
//<<<SAPPER SECTION END MAIN-UNKNOWN-EXCEPTION-HANDLER>>>
        return 1;
    }

    return 0;
}

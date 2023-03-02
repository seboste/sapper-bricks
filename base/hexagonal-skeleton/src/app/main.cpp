#include <core/core.h>
#include <memory>
#include <microservice-essentials/context.h>
#include <microservice-essentials/handler.h>
#include <microservice-essentials/utilities/environment.h>
#include <microservice-essentials/cross-cutting-concerns/exception-handling-request-hook.h>
#include <microservice-essentials/cross-cutting-concerns/graceful-shutdown.h>
#include <microservice-essentials/request/request-processor.h>
//<<<SAPPER SECTION BEGIN MAIN-INCLUDES>>>
//<<<SAPPER SECTION END MAIN-INCLUDES>>>

int main()
{
    mse::Context::GetGlobalContext().Insert({   
//<<<SAPPER SECTION BEGIN MAIN-ENVIRONMENT>>>
//<<<SAPPER SECTION END MAIN-ENVIRONMENT>>>
            {"app", mse::getenv_or("APP", "test-grpc") },
            {"version", mse::getenv_or("VERSION", "1.0.0") }
        });

//<<<SAPPER SECTION BEGIN MAIN-REQUEST-HOOKS>>>
    mse::RequestHandler::GloballyWith(mse::ExceptionHandlingRequestHook::Parameters{});
//<<<SAPPER SECTION END MAIN-REQUEST-HOOKS>>>
    
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

    return 0;
}

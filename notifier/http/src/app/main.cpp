//<<<SAPPER SECTION BEGIN MERGE MAIN-INCLUDES>>>
#include <adapters/notifier-http/notifier-http.h>
//<<<SAPPER SECTION END MERGE MAIN-INCLUDES>>>

int main()
{
        ...

        //<<<SAPPER SECTION BEGIN REPLACE MAIN-NOTIFIER-INSTANTIATION>>>
        std::unique_ptr<EntityNotifier> notifier = std::make_unique<HttpNotifier>(
                mse::getenv_or("NOTIFIER_CONNECTION_STRING", "<<<NOTIFIER_CONNECTION_STRING>>>"),
                mse::getenv_or("NOTIFIER_MAX_RETRY_COUNT", <<<NOTIFIER_MAX_RETRY_COUNT>>>),
                std::chrono::milliseconds(mse::getenv_or("NOTIFIER_FIRST_RETRY_INTERVAL_MS", <<<NOTIFIER_FIRST_RETRY_INTERVAL_MS>>>))
        );
        //<<<SAPPER SECTION END REPLACE MAIN-NOTIFIER-INSTANTIATION>>>        
    
        ...
}

//<<<SAPPER SECTION BEGIN MERGE MAIN-INCLUDES>>>
#include <adapters/notifier-pubsub/notifier-pubsub.h>
//<<<SAPPER SECTION END MERGE MAIN-INCLUDES>>>

int main()
{
        const std::vector<std::string> headers_to_propagate = {};

        ...

        //<<<SAPPER SECTION BEGIN REPLACE MAIN-NOTIFIER-INSTANTIATION>>>
        std::unique_ptr<EntityNotifier> notifier = std::make_unique<PubSubNotifier>(                
                mse::getenv_or("GOOGLE_PROJECT", "<<<GOOGLE_PROJECT>>>"), 
                mse::getenv_or("PUBSUB_TOPIC", "<<<PUBSUB_TOPIC>>>"),                
                headers_to_propagate
        );
        //<<<SAPPER SECTION END REPLACE MAIN-NOTIFIER-INSTANTIATION>>>        
    
        ...
}

//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/handler-pubsub/handler-pubsub.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
        ...

//<<<SAPPER SECTION BEGIN REPLACE MAIN-HANDLER-INSTANTIATION>>>
        PubSubHandler handler(core, mse::getenv_or("GOOGLE_PROJECT", "<<<GOOGLE_PROJECT>>>"), mse::getenv_or("PUBSUB_SUBSCRIPTION", "<<<PUBSUB_SUBSCRIPTION>>>"));
//<<<SAPPER SECTION END REPLACE MAIN-HANDLER-INSTANTIATION>>>

        ...
        
        handler.Handle();

    return 0;
}

//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/handler-http/handler-http.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
        ...

//<<<SAPPER SECTION BEGIN REPLACE MAIN-HANDLER-INSTANTIATION>>>
        HttpHandler handler(core, mse::getenv_or("HOST", "<<<HANDLER_HTTP_HOST>>>"), mse::getenv_or("PORT", <<<HANDLER_HTTP_PORT>>>));
//<<<SAPPER SECTION END REPLACE MAIN-HANDLER-INSTANTIATION>>>

        ...
        
        handler.Handle();

    return 0;
}

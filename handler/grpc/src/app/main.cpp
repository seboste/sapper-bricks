//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/handler-grpc/handler-grpc.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
        ...

//<<<SAPPER SECTION BEGIN REPLACE MAIN-HANDLER-INSTANTIATION>>>
        GrpcHandler handler(core, mse::getenv_or("HOST", "<<<HOST>>>"), mse::getenv_or("PORT", <<<PORT>>>));        
//<<<SAPPER SECTION END REPLACE MAIN-HANDLER-INSTANTIATION>>>

        ...

        handler.Handle();

    return 0;
}

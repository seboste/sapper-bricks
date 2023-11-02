//<<<SAPPER SECTION BEGIN MERGE MAIN-INCLUDES>>>
#include <adapters/repo-redis/repo-redis.h>
//<<<SAPPER SECTION END MERGE MAIN-INCLUDES>>>

int main()
{
        ...

        //<<<SAPPER SECTION BEGIN REPLACE MAIN-REPO-INSTANTIATION>>>
        std::unique_ptr<EntityRepo> repo = std::make_unique<RedisRepo>(mse::getenv_or("REDIS_CONNECTION_STRING", "<<<REDIS_CONNECTION_STRING>>>"));
        //<<<SAPPER SECTION END REPLACE MAIN-REPO-INSTANTIATION>>>
    
        ...
}

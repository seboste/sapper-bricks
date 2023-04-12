//<<<SAPPER SECTION BEGIN MERGE MAIN-INCLUDES>>>
#include <adapters/repo-postgres/repo-postgres.h>
//<<<SAPPER SECTION END MERGE MAIN-INCLUDES>>>

int main()
{
        ...

        //<<<SAPPER SECTION BEGIN REPLACE MAIN-REPO-INSTANTIATION>>>
        std::unique_ptr<EntityRepo> repo = std::make_unique<PostgresRepo>(mse::getenv_or("POSTGRES_CONNECTION_STRING", <<<POSTGRES_CONNECTION_STRING>>>));
        //<<<SAPPER SECTION END REPLACE MAIN-REPO-INSTANTIATION>>>
    
        ...
}

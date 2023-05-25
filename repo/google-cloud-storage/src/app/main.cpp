//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/repo-cloud-storage/repo-cloud-storage.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
    
//<<<SAPPER SECTION BEGIN REPLACE MAIN-REPO-INSTANTIATION>>>
        std::unique_ptr<EntityRepo> repo = std::make_unique<CloudStorageRepo>(mse::getenv_or("CLOUD_STORAGE_BUCKET", "<<<CLOUD_STORAGE_BUCKET>>>"));
//<<<SAPPER SECTION END REPLACE MAIN-REPO-INSTANTIATION>>>

        ...

        Core core(repo.get(), notifier.get());

        ...

    return 0;
}

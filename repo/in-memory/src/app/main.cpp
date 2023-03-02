//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/repo-in-memory/repo-in-memory.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
    
//<<<SAPPER SECTION BEGIN REPLACE MAIN-REPO-INSTANTIATION>>>
    std::unique_ptr<EntityRepo> repo = std::make_unique<InMemoryRepo>();

//<<<SAPPER SECTION END REPLACE MAIN-REPO-INSTANTIATION>>>

    ...

    Core core(repo.get(), notifier.get());

    ...

    return 0;
}

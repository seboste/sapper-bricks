add_library(repo-cloud-storage)

find_package(google-cloud-cpp)
find_package(nlohmann_json)

target_sources(repo-cloud-storage
    PUBLIC
        repo-cloud-storage.h
    PRIVATE
        repo-cloud-storage.cpp
    )
    
target_link_libraries(repo-cloud-storage
    PUBLIC
        ports
    PRIVATE
        nlohmann_json::nlohmann_json
        google-cloud-cpp::storage
    )


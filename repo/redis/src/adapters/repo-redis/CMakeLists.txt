add_library(repo-redis)

find_package(redis++)
find_package(nlohmann_json)

target_sources(repo-redis
    PUBLIC
        repo-redis.h
        repo-redis.cpp
    )
    
target_link_libraries(repo-redis
    PUBLIC
        ports
#    PRIVATE should compile, but doesn't
        redis++::redis++_static    
        nlohmann_json::nlohmann_json
    )

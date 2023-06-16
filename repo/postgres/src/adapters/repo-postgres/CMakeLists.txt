add_library(repo-postgres)

find_package(libpqxx)

target_sources(repo-postgres
    PUBLIC        
        repo-postgres.h
    PRIVATE
        impl/postgres-connection-manager.cpp
        impl/postgres-connection-manager.h
        repo-postgres.cpp
    )
    
target_link_libraries(repo-postgres
    PUBLIC
        ports
    PRIVATE
        libpqxx::pqxx
        microservice-essentials::microservice-essentials
    )

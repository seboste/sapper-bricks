find_package(libpqxx)
find_package(microservice-essentials)

target_sources(tests    
    PRIVATE
        impl/postgres-connection-manager_test.cpp
    )

target_link_libraries(tests
    PRIVATE
        repo-postgres
        libpqxx::libpqxx
        microservice-essentials::microservice-essentials
    )
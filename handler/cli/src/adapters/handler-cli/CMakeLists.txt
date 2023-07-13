add_library(handler-cli)

find_package(CLI11)
find_package(nlohmann_json)

target_sources(handler-cli
    PUBLIC
        handler-cli.h
    PRIVATE
        handler-cli.cpp
    )

target_link_libraries(handler-cli
    PUBLIC 
        ports
        microservice-essentials::microservice-essentials
    PRIVATE
        nlohmann_json::nlohmann_json
        CLI11::CLI11
    )

add_library(handler-pubsub)

find_package(google-cloud-cpp)
find_package(nlohmann_json)

target_sources(handler-pubsub
    PUBLIC
        handler-pubsub.h
    PRIVATE
        handler-pubsub.cpp
    )

target_link_libraries(handler-pubsub
    PUBLIC 
        ports
        microservice-essentials::microservice-essentials
    PRIVATE        
        nlohmann_json::nlohmann_json
        google-cloud-cpp::pubsub                
    )

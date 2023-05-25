add_library(notifier-pubsub)

find_package(google-cloud-cpp)
find_package(nlohmann_json)

target_sources(notifier-pubsub
    PUBLIC
        notifier-pubsub.h
    PRIVATE
        notifier-pubsub.cpp
    )

target_link_libraries(notifier-pubsub
    PUBLIC 
        ports
        microservice-essentials::microservice-essentials        
    PRIVATE
        google-cloud-cpp::pubsub
        nlohmann_json::nlohmann_json        
    )

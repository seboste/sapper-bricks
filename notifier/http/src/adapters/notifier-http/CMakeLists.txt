add_library(notifier-http)

find_package(httplib)
find_package(nlohmann_json)
find_package(OpenSSL)

target_sources(notifier-http
    PUBLIC
        notifier-http.h
    PRIVATE
        notifier-http.cpp
    )

target_link_libraries(notifier-http
    PUBLIC 
        ports
        microservice-essentials::microservice-essentials
    PRIVATE        
        httplib::httplib 
        nlohmann_json::nlohmann_json
        OpenSSL::OpenSSL
    )

add_library(handler-http)

find_package(httplib)
find_package(nlohmann_json)
find_package(OpenSSL)

target_sources(handler-http
    PUBLIC
        handler-http.h
    PRIVATE
        handler-http.cpp
    )

target_link_libraries(handler-http
    PUBLIC 
        ports
        microservice-essentials::microservice-essentials
    PRIVATE        
        httplib::httplib 
        nlohmann_json::nlohmann_json
        OpenSSL::OpenSSL
    )

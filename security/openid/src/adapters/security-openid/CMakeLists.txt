add_library(security-openid)
find_package(httplib)
find_package(nlohmann_json)
find_package(microservice-essentials)
find_package(OpenSSL)

target_sources(security-openid
    PUBLIC
        openid-configuration.h
    PRIVATE
        openid-configuration.cpp
    )

target_link_libraries(security-openid
    PUBLIC        
    PRIVATE
        httplib::httplib
        microservice-essentials::microservice-essentials
        nlohmann_json::nlohmann_json
        OpenSSL::OpenSSL
    )


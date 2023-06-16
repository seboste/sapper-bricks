add_library(security-jwt-auth)

find_package(cpp-jwt)
find_package(httplib)
find_package(OpenSSL)

target_sources(security-jwt-auth
    PUBLIC
        jwt-auth-request-hook.h
        rest-jwks-rsa-key-provider.h        
    PRIVATE
        jwt-auth-request-hook.cpp        
        rest-jwks-rsa-key-provider.cpp
        impl/key_conversion.h
        impl/key_conversion.cpp
    )

target_link_libraries(security-jwt-auth
    PUBLIC
        microservice-essentials::microservice-essentials
    PRIVATE
        cpp-jwt::cpp-jwt
        httplib::httplib
        openssl::openssl
    )

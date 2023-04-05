target_sources(tests
    PRIVATE
        impl/key_conversion_test.cpp
        jwt-auth-request-hook_test.cpp
        rest-jwks-rsa-key-provider_test.cpp
    )

target_link_libraries(tests
    PRIVATE
        security-jwt-auth
    )
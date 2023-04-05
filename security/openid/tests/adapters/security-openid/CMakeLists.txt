target_sources(tests
    PRIVATE
        openid-configuration_test.cpp
    )

target_link_libraries(tests
    PRIVATE
        security-openid
    )

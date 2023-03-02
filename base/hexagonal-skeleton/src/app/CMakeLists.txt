add_executable(<<<NAME>>>)

target_sources(<<<NAME>>>
    PRIVATE
        main.cpp
    )

target_link_libraries(<<<NAME>>>
    PUBLIC
#<<<SAPPER SECTION BEGIN APP-DEPENDENCIES>>>
        core
        microservice-essentials::microservice-essentials
#<<<SAPPER SECTION END APP-DEPENDENCIES>>>
    )

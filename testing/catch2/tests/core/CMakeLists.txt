target_sources(tests  
    PRIVATE
        core_test.cpp
    )

target_link_libraries(tests 
    PRIVATE
        core
    )
add_library(repo-in-memory)

target_sources(repo-in-memory
    PUBLIC
        repo-in-memory.h
        repo-in-memory.cpp
    )
    
target_link_libraries(repo-in-memory
    PUBLIC
        ports        
    )

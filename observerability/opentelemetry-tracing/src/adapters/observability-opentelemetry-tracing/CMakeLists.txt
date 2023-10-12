find_package(opentelemetry-cpp REQUIRED)

add_library(observability-opentelemetry-tracing)

target_sources(observability-opentelemetry-tracing
    PUBLIC        
        tracing-request-hook.h        
    PRIVATE        
        tracing-request-hook.cpp        
        impl/mse-context-carrier.h
        impl/mse-context-carrier.cpp
    )

target_link_libraries(observability-opentelemetry-tracing
    PUBLIC
        microservice-essentials::microservice-essentials 
    PRIVATE
        opentelemetry-cpp::opentelemetry-cpp
    )

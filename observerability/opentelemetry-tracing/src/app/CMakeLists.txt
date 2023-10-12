
target_link_libraries(the-application
    PUBLIC
#<<<SAPPER SECTION BEGIN MERGE APP-DEPENDENCIES>>>
        observability-opentelemetry-tracing
#<<<SAPPER SECTION END MERGE APP-DEPENDENCIES>>>
    )

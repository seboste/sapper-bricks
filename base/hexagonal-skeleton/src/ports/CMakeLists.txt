add_library(ports INTERFACE)
target_sources(ports
    INTERFACE
        api.h
        model.h
        repo.h
        notifier.h
    )
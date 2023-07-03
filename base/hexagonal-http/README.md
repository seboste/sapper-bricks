# base-hexagonal-http

This *service template* is based on *bease-hexagonal-skeleton* service template and adds a http handler to provide a REST API.

## usage

Create a new *base hexagonal http* service with the following command:

```bash
sapper service add --template base-hexagonal-http my-service
```

See the *handler-http* brick documentation for an overview on how to use the service.

## parameters

See *handler-http* and *base-hexagonal-http* for a description of the parameters.

## next steps

Some possible next steps is the addition of a repository and structured logging. This can be done with the following commands:
```bash
sapper brick add repo-in-memory
sapper brick add observability-structured-logging
```

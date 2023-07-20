# base-hexagonal-http

The *hexagonal-skeleton* template brick provides the basic folder structure for a new microservice with [hexagonal](https://en.wikipedia.org/wiki/Hexagonal_architecture_(software)) aka *ports and adapters* architecture. Furthermore, it provides [CMake](https://cmake.org/), [conan](https://conan.io/), and [GNU make](https://www.gnu.org/software/make/manual/make.html) files, which contain *sapper* sections for customization by *sapper bricks* that can be added later on.

For illustration purposes, the service already has some business logic around an example-*Entity* that many of the sapper bricks rely on. It also comes with some basic features, in part being provided by the [microservice essentials](https://github.com/seboste/microservice-essentials) library. The following table describes those features along with some basic concepts based on the folder structure of the *src* folder:

| Layer/folder | Desription| Depends on |
| :------ | :-----| :---- |
| ports | Domain entities and **interfaces** that the core needs to work with. As an example, it defines a simple struct named *Entity* and Interfaces for an *Entity*-Repository, an *Entity*-Notifier, and an API to set and get an *Entity*.   | - |
| core | Contains all **business logic** without directly depending on any infrastructure (e.g. databases). Here, the API defined in *ports* is implemented. The set-method validates the input, stores an *Entity* in a repository, and sends a notification that the *Entity* has changed. The get-method simply looks up an *Entity* in a repository.  | ports |
| adapters | **Implementation** of the interfaces defined in the ports layer such as the *Entity*-Repository or -Notifier.  Adapters should be independent from each other, but often rely on third party libraries. This folder is initially empty. *Sapper* bricks for specific technologies (e.g. a gRPC handler or a Postgres-Repository) often provide an adapter. | ports, external libraries|
| app | The **main** application that brings everything together: Reads configuration, instantiates the core, instantiates the adapters, injects the adapters into the core, calls the handler. The *main.cpp* contains many *sapper* sections that are modified whenever a new *sapper* brick is added. Furthermore, it instantiates a global [exception handler](https://raw.githubusercontent.com/seboste/microservice-essentials/main/src/microservice-essentials/cross-cutting-concerns/exception-handling-request-hook.h) that is able translate typical standard exceptions into request status codes and [gracefully shuts down](https://raw.githubusercontent.com/seboste/microservice-essentials/main/src/microservice-essentials/cross-cutting-concerns/graceful-shutdown.h) the service when a SIGTERM signal is received. | core, adapters |

## usage

As the *hexagonal-skeleton" template brick is the default template, a new service named *my-service* can be created with the following command:

```bash
sapper service add my-service
```

A new directory named `my-service` has been created.
You can then `cd` into that directory and run further sapper commands such as `sapper service build .` or `sapper brick add <brick-name>`.

## parameters
- **NAME**: The name of the service. Both, the executable, and the service folder will get that name.
- **VERSION**: The current version of the service. It is recommended to follow [semantic versioning](https://semver.org/).

Both parameters can be obtained from the global context using ```mse::Context::GetGlobalContext().At("app" /* or "version"*/)```, e.g. for logging.

## next steps

The next steps are certainly adding bricks to your newly created service: Handler, repository, observability.
This can for example be achieved with the following commands:
```bash
sapper brick add handler-http
sapper brick add repo-postgres
sapper brick add observability-structured-logging
```
Then run your service with the command `sapper service run .` and issue http commands to it using *curl*.

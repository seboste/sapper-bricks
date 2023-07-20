# repo-in-memory

Adds a simple key value store based on *std::map* to the service . This serves as a repository for storing and retrieving the service's example Entity.

> **_WARNING:_** This brick is intended for testing-/development- purposes only because it adds state to an instance. This prevents horizontal scaling and therefore shall be avoided.

## open issues

> **_WARNING:_**  The Implementation is not thread-safe. Memory corruption may occur.

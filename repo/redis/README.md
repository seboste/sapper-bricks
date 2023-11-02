# repo-redis

Repo that stores and retrieves the service's example entities to and from the popular [Redis](https://redis.com/) database as `Redis Strings` where the key is the id of the entity and the value is a JSON representation of the entity.

Furthermore, the `Makefile` defines a target *dbstart* that spins up a *redis-stack* docker container. If you want to use this, make sure to have [docker](https://www.docker.com/) installed on your system and the docker engine is up and running.

The brick uses the recommended [redis-plus-plus](https://github.com/sewenew/redis-plus-plus) client library.

## usage

If you don't already have a redis instance up and running, you can confirm the default REDIS_CONNECTION_STRING parameter. Make sure to have [docker](https://www.docker.com/) installed and running. Then simply run the following command:

```bash
sapper service run .
```

The *run* target in the `Makefile` depends on the *dbstart* target mentioned earlier. Thus, before eventually starting the service, a *redis-stack* container is being run to which one can connect with that default connection string. If you havn't done so before, add a handler of your choice. Then, the requests sent to your service will access the *redis* db. To explore the data and configure you redis instance, you can navigate to the [redis GUI](http://localhost:8001/) with a browser of your choice.

## parameters

  - **REDIS_CONNECTION_STRING**: Defines host, port, and credentials for connecting to the database in a [connection string format](https://www.iana.org/assignments/uri-schemes/prov/redis). Default: `redis://default:my_password@localhost:6379/0`. 


## next steps

- For persisting data other than the example entity, the *RedisRepo* class should be changed according to your repo interface in the *ports* folder.
- For security reasons make sure that the connection string including the credentials is not stored in code. Instead, consider containerizing your service (e.g. by using the *deploy-docker* brick) and pass the connection string as an environment variable named `REDIS_CONNECTION_STRING`. Consider using *google secret manager* or something similar to store the connection string.

## open issues

*-*

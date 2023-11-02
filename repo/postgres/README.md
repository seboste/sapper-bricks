# repo-postgres

Repo that stores and retrieves the service's example entities to and from the popular [PostgreSQL](https://www.postgresql.org/) database. Each entity corresponds to one row in a table with its id as the primary key.

The brick defines the database schema in `db/schema.sql` that can be used to initialize a PostgreSQL database instance. Furthermore, the `Makefile` defines a target *dbstart* that spins up a *postgres* docker container with the appropriate db layout. If you want to use this, make sure to have [docker](https://www.docker.com/) installed on your system and the docker engine is up and running.

The brick uses the **libpqxx** library for sending prepared *SELECT*, *INSERT*, and *UPDATE* statements to a postgresDB specified by a connection string. The brick comes with a *ConnectionManager* class that provides retries with exponential backups for (re-) establishing a connection to the database.

## usage

If you don't already have a postgreSQL instance up and running, you can confirm the default POSTGRES_CONNECTION_STRING parameter. Make sure to have [docker](https://www.docker.com/) installed and running. Then simply run the following command:

```bash
sapper service run .
```

The *run* target in the `Makefile` depends on the *dbstart* target mentioned earlier. Thus, before eventually starting the service, a *postgres* container is being run to which one can connect with that default connection string. If you havn't done so before, add a handler of your choice. Then, the requests sent to your service will access the *postgres* db.

## parameters

  - **POSTGRES_CONNECTION_STRING**: Defines host, port, and credentials for connecting to the database in a [connection string format](https://www.postgresql.org/docs/current/libpq-connect.html#LIBPQ-CONNSTRING). Default: `postgres://postgres:my_password@localhost:5432`. 


## next steps

- For persisting data other than the example entity, the *PostgresRepo* class should be changed according to your repo interface in the *ports* folder.
- For security reasons make sure that the connection string including the credentials is not stored in code. Instead, consider containerizing your service (e.g. by using the *deploy-docker* brick) and pass the connection string as an environment variable named `POSTGRES_CONNECTION_STRING`. Consider using *google secret manager* or something similar to store the connection string.

## open issues

*-*

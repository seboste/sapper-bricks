# handler-cli

This handler provides a command line interface (CLI) with *get* and *set* subcommands with which the service's example entity can be accessed. It uses the **cli11** library. This handler can be useful in a development scenario, e.g. for debugging.

## usage

Simply start the service with the *set* or *get* subcommand followed by the entity to be set as JSON or the id of the entity to be returned on standard output as JSON, respectively: 
```bash
./build/src/app/cli-test set '{ "id": "dd2aebfc-21ad-11ee-be56-0242ac120002", "string_property": "test", "int_property": 123 }'
./build/src/app/cli-test get dd2aebfc-21ad-11ee-be56-0242ac120002
```
Beware that the process ends after each call. Therefore, the service must be stateless, i.e. the repo of the service where all the entities are stored must not use the memory of the process as for example the *repo-in-memory* brick would do. Otherwise the just stored entity with the *set* subcommand will be lost immediately.

In case you need to pass metadata as key value pairs to the request's context (e.g. for authentication or tracing), you can use the `--metadata [key,value]` command line option. For basic authentication, a full example command could look like:

```bash
./build/src/app/cli-test --metadata [authorization,secret-token] set '{ "id": "dd2aebfc-21ad-11ee-be56-0242ac120002", "string_property": "test", "int_property": 123 }'
```

## parameters

*-*

## next steps

*-*

## open issues

*-*
 
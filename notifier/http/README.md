# notifier-http

Whenever, one of the example entities changes, this brick sends a PUT http request to the `/Entity/<entity-id>` endpoint of a predefined server. The body contains the changed entity in JSON format. To increase reliability, retries with an exponential backoff strategy are implemented.

It uses **cpp-httplib** for making the request and **nlohmann_json** for converting entities to json.

## usage

You can setup two services, one with an http-handler and an http-notifier, and the other one with just an http-handler that listens on a different port (e.g. 8080 and 8081). The first service needs to be configured so the the notifier calls the other service.
Once both services are started, you can use *curl* to set and get entities:

```bash
curl -X PUT localhost:8080/Entity/test -H "Content-Type: application/json" -d'{"id":"test","string_property":"str","int_property":42}'

curl localhost:8081/Entity/test
```
Note that the first request goes to the first service, i.e. the one with port 8080 and the http-notifier. But the second request goes to the second service, i.e. the one with port 8081 which receives the notifications from the first service.

## parameters

- **NOTIFIER_CONNECTION_STRING**: Specifies to where the notifications should be sent to. It has the format `<protocol>://<host>:<port>`. The default is `http://localhost:8080`.
- **NOTIFIER_MAX_RETRY_COUNT**: The maximum number of retries in case of failure. The default is `3`.
- **NOTIFIER_FIRST_RETRY_INTERVAL_MS**: The duration in ms after which the first retry will happen. Subsequent calls are delayed with an exponentially increasing duration to which some random duration is added to avoid a [retry storm](https://learn.microsoft.com/en-us/azure/architecture/antipatterns/retry-storm/). Default: `100ms`.

## next steps

For notifying endpoints other than the example entity, the *HttpNotifier* class should be changed according to your *API*.

## open issues

Https support is currently not implemented. Don't use it for sensitive data.
 
# handler-http

This handler provides a REST interface with get and put endpoints for the service's example entity. It uses **cpp-httplib** for setting up an http server and **nlohmann_json** for converting entities from and to json.

## usage

Once the service is started with the http-handler, you can use *curl* to set and get entities:

```bash
curl -X PUT localhost:8080/Entity/test -H "Content-Type: application/json" -d'{"id":"test","string_property":"str","int_property":42}'

curl localhost:8080/Entity/test
```

## parameters

- **HOST**: The ip-address on which the http server listens. The default is *0.0.0.0*, i.e. the server listens to all possible hosts.
- **PORT**: The port which the http server binds to. The default is *8080*.

## next steps

For providing endpoints other than the example entity, the *HttpHandler* class should be changed according to your *API*.

## open issues

Https support is currently not implemented. Don't use it for sensitive data.
 
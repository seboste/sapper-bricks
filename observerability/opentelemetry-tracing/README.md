# observability-opentelemetry-tracing

This brick adds distributed tracing based on [**OpenTelemetry**](https://opentelemetry.io/) to your service. Each request has a unique trace id that is used to follow a request through a set of microservices. For each incoming and outgoing request a span associated with that trace is created. The trace id is passed from one service to another, e.g. using header for http calls or metadata for gRPC calls. All traces are sent to tracing platform that supports opentelemetry such as [**Jaeger**](https://www.jaegertracing.io/) for monitoring the services.

## usage

First, start a tracing platform, e.g. by running 
```
docker run --rm --name jaeger \
  -e COLLECTOR_ZIPKIN_HOST_PORT=:9411 \
  -p 6831:6831/udp \
  -p 6832:6832/udp \
  -p 5778:5778 \
  -p 16686:16686 \
  -p 4317:4317 \
  -p 4318:4318 \
  -p 14250:14250 \
  -p 14268:14268 \
  -p 14269:14269 \
  -p 9411:9411 \
  jaegertracing/all-in-one:1.49
```
When adding the brick make sure to set the `OPENTELEMETRY_HOST` and `OPENTELEMETRY_PORT` parameters so that they point to the opentelemetry tracing platform. Then, open the UI of your tracing platform (e.g. by navigating to [http://localhost:16686/](http://localhost:16686/)).
Whenever you send a request to your service, a new trace including all spans should be visible in that UI.

## parameters

  - **OPENTELEMETRY_HOST**: The host of the URL to which tracing information shall be sent to. The complete URL has the form of `http://<OPENTELEMETRY_HOST>:<OPENTELEMETRY_PORT>/v1/traces` and can be overriden by the environment variable `OPENTELEMETRY_URL`. The Default for the OPENTELEMETRY_HOST is `localhost`.
  - **OPENTELEMETRY_PORT**: The port of the URL to which tracing information shall be sent to. The complete URL has the form of `http://<OPENTELEMETRY_HOST>:<OPENTELEMETRY_PORT>/v1/traces` and can be overriden by the environment variable `OPENTELEMETRY_URL`. The Default for the OPENTELEMETRY_PORT is `4318`.
  - **OPENTELEMETRY_PROPAGATION_TYPE**: The kind of propagation header that shall be used to transport tracing information accross service boundaries. Can be either `B3_SingleHeader` (metadata b3: <TraceId>-<SpanId>-1-<ParentSpanID>), `B3_MultiHeader` (metadata X-B3-TraceId: <TraceId>; X-B3-SpanId: <SpanId>; X-B3-ParentSpanId: <ParentSpanID>), or `Http` (metadata traceparent; tracestate). Default `B3_SingleHeader`.  

## next steps

`-`

## open issues

`-`

# observability-structured-logging

This brick adds structured logging to your service. Log messages are written to standard output in a JSON format. The fields in that JSON can be configured. The logging functionality is provided by the [**microservice-essentials**](https://github.com/seboste/microservice-essentials) library. It offers logging Macros such as `MSE_LOG_INFO` for logging and obtains the values of the fields from the global (e.g. *app*), the request local (e.g. *x-b3-traceid*), the local (e.g. *file*, *line*, *timestamp*), and the user defined (e.g. *message*, *level*) context.

## usage

Use the `LOG_*` macors defined by the microservice-essentials' `logger.h` to obtain structured logs.

> **_WARNING:_**  Be careful to not log any sensitive data.

## parameters

  - **STRUCTURED_LOG_FIELDS**: The fields as a `const char*` initializer list that shall be written into the JSON log message. The values are obtained from the current context. Default `{"timestamp", "level", "app", "x-b3-traceid", "x-b3-spanid", "message"}`.

## next steps

Deploy your service to a managed kubernetes cluster of your preferred cloud provider. The cloud provider's log collector will forward the logs to their managed monitoring solution where the log files can be filtered and analyzed.


## open issues

`-`
# handler-grpc

This handler provides a [gRPC](https://grpc.io/) interface with *GetEntity* and *SetEntity* methods defined in the *api.proto* with which the service's example entity can be accessed. It uses **grpc** and the **protobuf** libraries. Based on the *api.proto* file, code is automatically generated during the build process.

## usage

Once the service is started with the grpc-handler, you can use a tool such as [BloomRPC](https://github.com/bloomrpc/bloomrpc) to set and get entities. Simply import the *api.proto* file, connect to *localhost* and you can set and get entities.

## parameters

- **HOST**: The ip-address on which the gRPC server listens. The default is *0.0.0.0*, i.e. the server listens to all possible hosts.
- **PORT**: The port which the gRPC server binds to. The default is *50051*.

## next steps

For providing endpoints other than the example entity, the *api.proto* file and the *GrpcHandler* class should be changed according to your *API*.

## open issues

Https support is currently not implemented. Don't use it for sensitive data.
 
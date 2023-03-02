#include "handler-grpc.h"
#include <api.grpc.pb.h>
#include <grpcpp/grpcpp.h>
#include <microservice-essentials/handler.h>
#include <microservice-essentials/context.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/request/request-processor.h>
#include <microservice-essentials/utilities/metadata-converter.h>
#include <microservice-essentials/utilities/status-converter.h>


using namespace grpc;

class GrpcHandler::Impl : public EntityPackage::EntityService::Service, public mse::Handler
{
public:
    Impl(Api& api, const std::string& host, int port)
        : _api(api)
    {
        const std::string server_address(host + ":" + std::to_string(port));
        _serverBuilder.AddListeningPort(server_address, InsecureServerCredentials());
        _serverBuilder.RegisterService(this);
    }

    virtual void Handle() override
    {
        MSE_LOG_INFO("serving");
        _server = _serverBuilder.BuildAndStart();
        _server->Wait();
    }

    virtual void Stop() override
    {
        MSE_LOG_INFO("stop requested");
        _server->Shutdown();
    }

    virtual Status GetEntity(::grpc::ServerContext* context, const ::EntityPackage::GetEntityRequest* request, ::EntityPackage::GetEntityResponse* response) override
    {
        return mse::ToGrpcStatus<grpc::Status>(
            mse::RequestHandler("GetEntity", mse::Context(mse::ToContextMetadata(context->client_metadata())))
                .Process([&](mse::Context&)
                {
                    const Entity e =  _api.GetEntity(request->id());
                    response->mutable_entity()->set_id(e.id);
                    response->mutable_entity()->set_string_property(e.string_property);
                    response->mutable_entity()->set_int_property(e.int_property);
                    return mse::Status::OK;
                })
            );   

    }

    virtual Status SetEntity(::grpc::ServerContext* context, const ::EntityPackage::SetEntityRequest* request, ::EntityPackage::SetEntityResponse* response) override
    {
        return mse::ToGrpcStatus<grpc::Status>(
            mse::RequestHandler("SetEntity", mse::Context(mse::ToContextMetadata(context->client_metadata())))
                .Process([&](mse::Context&)
                {                    
                    _api.SetEntity(Entity{ request->entity().id(), request->entity().string_property(), request->entity().int_property() });
                    return mse::Status::OK;
                })
        );        
    }

private:
    Api& _api;
    std::unique_ptr<Server> _server;
    ServerBuilder _serverBuilder;
};

GrpcHandler::GrpcHandler(Api& api, const std::string& host, int port)
    : _impl(std::make_unique<Impl>(api, host, port))
    , Handler("grpc-handler")
{
}

GrpcHandler::~GrpcHandler()
{
}

void GrpcHandler::Handle()
{
    _impl->Handle();
}

void GrpcHandler::Stop()
{
    _impl->Stop();
}
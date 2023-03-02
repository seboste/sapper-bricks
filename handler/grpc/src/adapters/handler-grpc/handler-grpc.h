#pragma once

#include <ports/api.h>
#include <microservice-essentials/handler.h>
#include <microservice-essentials/utilities/environment.h>
#include <memory>
#include <string>

class GrpcHandler : public mse::Handler
{
public:
    GrpcHandler(Api& api, const std::string& host = mse::getenv_or("HOST", "0.0.0.0"), int port = mse::getenv_or("PORT", 50051));
    virtual ~GrpcHandler();

    virtual void Handle() override;
    virtual void Stop() override;
private:

    class Impl;
    std::unique_ptr<Impl> _impl;
};

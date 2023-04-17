#pragma once

#include <ports/api.h>
#include <microservice-essentials/handler.h>
#include <memory>
#include <string>

class GrpcHandler : public mse::Handler
{
public:
    GrpcHandler(Api& api, const std::string& host, int port);
    virtual ~GrpcHandler();

    virtual void Handle() override;
    virtual void Stop() override;
private:

    class Impl;
    std::unique_ptr<Impl> _impl;
};

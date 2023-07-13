#pragma once

#include <ports/api.h>
#include <microservice-essentials/handler.h>
#include <memory>
#include <map>
#include <string>

class CliHandler : public mse::Handler
{
public:
    CliHandler(Api& api, int argc, char **argv);     
    virtual ~CliHandler();

    virtual void Handle() override;
    virtual void Stop() override;
private:

    std::string getEntity(const std::string& id, const std::map<std::string, std::string>& metadata) const;
    void setEntity(const std::string& data, const std::map<std::string, std::string>& metadata);

    Api& _api;
    int _argc; 
    char** _argv;
};

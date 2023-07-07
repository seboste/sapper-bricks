#pragma once

#include <ports/api.h>
#include <microservice-essentials/handler.h>
#include <microservice-essentials/context.h>
#include <string>
#include <mutex>
#include <condition_variable>

class PubSubHandler : public mse::Handler
{
public:
    PubSubHandler(Api& api, const std::string& google_project_id, const std::string& subscription_id);
    virtual ~PubSubHandler();

    virtual void Handle() override;
    virtual void Stop() override;
private:
    void setEntity(const std::string& message, const mse::Context::Metadata& metadata);
    
    Api& _api;    
    const std::string _google_project_id;
    const std::string _subscription_id;    
    
    bool _stop_requested = false;
    std::mutex _stop_mutex;
    std::condition_variable _stop_cv;    
};

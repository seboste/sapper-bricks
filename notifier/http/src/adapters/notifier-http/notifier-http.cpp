#include "notifier-http.h"
#include <microservice-essentials/context.h>
#include <microservice-essentials/cross-cutting-concerns/error-forwarding-request-hook.h>
#include <microservice-essentials/reliability/retry-request-hook.h>
#include <microservice-essentials/request/request-processor.h>
#include <microservice-essentials/utilities/metadata-converter.h>
#include <microservice-essentials/utilities/status-converter.h>
#include <microservice-essentials/utilities/url.h>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>

namespace {
using json = nlohmann::json;
json to_json(const Entity& entity)
{
    return json { 
        { "id", entity.id },
        { "string_property", entity.string_property },
        { "int_property", entity.int_property }
    };
}
}

HttpNotifier::HttpNotifier(const std::string& notification_connection_string, uint32_t max_retry_count, std::chrono::system_clock::duration first_retry_interval)
    : _notification_connection_string(notification_connection_string)
    , _max_retry_count(max_retry_count)
    , _first_retry_interval(first_retry_interval)
{   
}

HttpNotifier::~HttpNotifier()
{
}

void HttpNotifier::EntityChanged(const Entity& entity)
{    
    mse::RequestIssuer("entityChanged", mse::Context())
        .With(mse::ErrorForwardingRequestHook::Parameters().IncludeAllErrorCodes()) //forward all error codes to handler via exception
        .With(mse::RetryRequestHook::Parameters(std::make_shared<mse::BackoffGaussianJitterDecorator>(
             std::make_shared<mse::ExponentialRetryBackoff>(_max_retry_count, _first_retry_interval), 
             _first_retry_interval / 10.0)))
        .Process([&](mse::Context& context)
        {            
            httplib::Client client(_notification_connection_string);
            client.set_connection_timeout(3, 0);
            client.set_read_timeout(3, 0);            
            auto resp = client.Put(
                std::string("/Entity/") + entity.id,
                mse::FromContextMetadata<httplib::Headers>(context.GetMetadata()), 
                to_json(entity).dump(), 
                "application/json"
                );
            if(resp.error() != httplib::Error::Success)
            {
                return mse::Status{mse::StatusCode::unavailable, "received error while notifying"};
            }            
            return mse::Status{mse::FromHttpStatusCode(resp->status), ""};
        });        
}
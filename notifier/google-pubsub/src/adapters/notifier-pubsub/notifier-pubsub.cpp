#include "notifier-pubsub.h"
#include <microservice-essentials/context.h>
#include <microservice-essentials/cross-cutting-concerns/error-forwarding-request-hook.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/request/request-processor.h>
#include <microservice-essentials/utilities/metadata-converter.h>
#include <microservice-essentials/utilities/status-converter.h>
#include <google/cloud/pubsub/publisher.h>
#include <nlohmann/json.hpp>

namespace pubsub = ::google::cloud::pubsub;

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

PubSubNotifier::PubSubNotifier(const std::string& google_project_id, const std::string& pubsub_topic, const std::vector<std::string>& headers_to_propagate)
    : _connection(pubsub::MakePublisherConnection(pubsub::Topic(google_project_id, pubsub_topic)))
    , _headers_to_propagate(headers_to_propagate)
{
}

PubSubNotifier::~PubSubNotifier()
{
}

void PubSubNotifier::EntityChanged(const Entity& entity)
{    
    mse::RequestIssuer("entityChanged", mse::Context())
        .With(mse::ErrorForwardingRequestHook::Parameters().IncludeAllErrorCodes()) //forward all error codes to handler via exception        
        .Process([&](mse::Context& context)
        {       
            pubsub::Publisher publisher(_connection);            
            auto metadata_to_propagate = context.GetFilteredMetadata(_headers_to_propagate);            
            auto id = publisher.Publish(pubsub::MessageBuilder{}
                .SetData(to_json(entity).dump())
                .SetAttributes(metadata_to_propagate.begin(), metadata_to_propagate.end())
                .Build()).get();
            if(!id)            
            {
                return mse::Status{ mse::FromGrpcStatusCode(id.status().code()) , id.status().message() };
            }

            MSE_LOG_TRACE(std::string("published pubsub message with id ") +  id.value());
            return mse::Status::OK;
        });
}
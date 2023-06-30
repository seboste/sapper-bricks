#pragma once

#include <ports/notifier.h>
#include <string>
#include <vector>
#include <memory>

namespace google { namespace cloud { namespace pubsub { inline namespace v2_12 { class PublisherConnection; }}}}

class PubSubNotifier : public EntityNotifier
{
public:
    PubSubNotifier(
        const std::string& google_project_id, 
        const std::string& pubsub_topic,
        const std::vector<std::string>& headers_to_propagate       //which headers shall be propagated from this service to the notified service? (e.g. tracing headers or authorization headers)
        );

    virtual ~PubSubNotifier();

    virtual void EntityChanged(const Entity& entity) override;
private:
    std::shared_ptr<google::cloud::pubsub::PublisherConnection> _connection;
    std::vector<std::string> _headers_to_propagate;    
};

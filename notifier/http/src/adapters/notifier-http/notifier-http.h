#pragma once

#include <ports/notifier.h>
#include <chrono>
#include <string>
#include <vector>

class HttpNotifier : public EntityNotifier
{
public:
    HttpNotifier(
        const std::string& notification_connection_string,          //which url shall be notified at the "/Entity/<entity-id>" endpoint? (e.g. http://localhost:8081)
        const std::vector<std::string>& headers_to_propagate,       //which headers shall be propagated from this service to the notified service? (e.g. tracing headers or authorization headers)
        uint32_t max_retry_count,                                   //how often to retry in case of error?
        std::chrono::system_clock::duration first_retry_interval    //how long to wait until the first retry? (subsequent retries will have an exponentially growing retry interval)
        );

    virtual ~HttpNotifier();

    virtual void EntityChanged(const Entity& entity) override;
private:
    std::string _notification_connection_string;
    std::vector<std::string> _headers_to_propagate;
    uint32_t _max_retry_count;
    std::chrono::system_clock::duration _first_retry_interval;    
};

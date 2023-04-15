#pragma once

#include <ports/notifier.h>
#include <chrono>
#include <string>

class HttpNotifier : public EntityNotifier
{
public:
    HttpNotifier(const std::string& notification_connection_string, uint32_t max_retry_count, std::chrono::system_clock::duration first_retry_interval);
    virtual ~HttpNotifier();

    virtual void EntityChanged(const Entity& entity) override;
private:
    std::string _notification_connection_string;
    uint32_t _max_retry_count;
    std::chrono::system_clock::duration _first_retry_interval;
};

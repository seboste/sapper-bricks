#pragma once

#include <ports/repo.h>
#include <memory>
#include <string>

namespace sw { namespace redis { class Redis; }}

class RedisRepo : public EntityRepo
{
public:
    RedisRepo(const std::string& connection_string);    
    virtual ~RedisRepo();
    
    virtual void Store(const Entity& entity) override;
    virtual std::optional<Entity> Retrieve(const std::string& id) const override;
private:
    std::unique_ptr<sw::redis::Redis> _connection;
};

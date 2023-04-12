#pragma once

#include <ports/repo.h>
#include <chrono>
#include <memory>

class PostgresConnectionManager;

class PostgresRepo : public EntityRepo
{
public:
    PostgresRepo(const std::string& connection_string, uint32_t max_retry_count = 3, std::chrono::system_clock::duration first_retry_interval = std::chrono::milliseconds(200));
    virtual ~PostgresRepo();
    
    virtual void Store(const Entity& entity) override;
    virtual std::optional<Entity> Retrieve(const std::string& id) const override;
private:    
    std::unique_ptr<PostgresConnectionManager> _connection_manager;    
};

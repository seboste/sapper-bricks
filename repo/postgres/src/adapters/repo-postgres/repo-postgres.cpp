#include "repo-postgres.h"
#include "impl/postgres-connection-manager.h"
#include <microservice-essentials/observability/logger.h>
#include <memory>

namespace {

std::unique_ptr<pqxx::connection> CreateConnection(const std::string& connection_string)
{
    std::unique_ptr<pqxx::connection> connection = std::make_unique<pqxx::connection>(connection_string);
    connection->prepare("store_entity", "INSERT INTO entity.entity(id, string_property, int_property) VALUES ($1,$2,$3) ON CONFLICT (id) DO UPDATE SET string_property = EXCLUDED.string_property, int_property = EXCLUDED.int_property;");
    connection->prepare("retrieve_entity", "SELECT id, string_property, int_property FROM entity.entity WHERE id = $1");
    return connection;
}

}

PostgresRepo::PostgresRepo(const std::string& connection_string, uint32_t max_retry_count, std::chrono::system_clock::duration first_retry_interval)
    : _connection_manager(std::make_unique<PostgresConnectionManager>([=]() { return CreateConnection(connection_string);}, max_retry_count, first_retry_interval))
{   
}

PostgresRepo::~PostgresRepo()
{
}
   
void PostgresRepo::Store(const Entity& entity)
{
    _connection_manager->WithConnection([&](pqxx::connection& c)
    {        
        pqxx::work t{c};
        t.exec_prepared("store_entity", entity.id, entity.string_property, entity.int_property);
        t.commit();
    }, "store");
}

std::optional<Entity> PostgresRepo::Retrieve(const std::string& id) const
{
    std::optional<Entity> entity_opt = std::nullopt;
    _connection_manager->WithConnection([&](pqxx::connection& c)
    {
        pqxx::work t{c};
        pqxx::result result = t.exec_prepared("retrieve_entity", id);
        if(result.size() == 1)
        {
            Entity e;
            std::tie(e.id, e.string_property, e.int_property) = result[0].as<std::string,std::string,int>();
            entity_opt = e;
        }
    }, "retrieve");
    return entity_opt;
}

#include "repo-redis.h"
#include <exception>
#include <nlohmann/json.hpp>
#include <sw/redis++/redis++.h>


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

void from_json(const json& entity_json, Entity& e)
{    
    entity_json.at("id").get_to(e.id);
    entity_json.at("string_property").get_to(e.string_property);
    entity_json.at("int_property").get_to(e.int_property);
}

}


RedisRepo::RedisRepo(const std::string& connection_string)
: _connection(std::make_unique<sw::redis::Redis>(connection_string))
{
    if(_connection->ping("test") != "test")
    {
        throw std::runtime_error("[redis] invalid connection");
    }
}

RedisRepo::~RedisRepo()
{
}


void RedisRepo::Store(const Entity& entity)
{
    if(!_connection->set(entity.id, to_json(entity).dump()))
    {
        throw std::runtime_error("[redis] unable to set entity");
    }
}

std::optional<Entity> RedisRepo::Retrieve(const std::string& id) const
{
    std::optional<std::string> json = _connection->get(id);
    if(!json.has_value())
    {
        return std::nullopt;
    }

    Entity e;    
    from_json(json::parse(json.value()), e);
    return e;
}

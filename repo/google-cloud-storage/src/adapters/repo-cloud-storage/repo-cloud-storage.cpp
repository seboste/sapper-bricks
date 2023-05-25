#include "repo-cloud-storage.h"
#include <nlohmann/json.hpp>
#include <google/cloud/storage/client.h>

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

namespace gcs = google::cloud::storage;
using ::google::cloud::StatusOr;

CloudStorageRepo::CloudStorageRepo(const std::string& bucket)
    : _bucket(bucket)
{
}

void CloudStorageRepo::Store(const Entity& entity)
{
    StatusOr<gcs::Client> client = gcs::Client::CreateDefaultClient();
    if (!client)
    {
        throw std::runtime_error("unable to create cloud storage client");
    }

    gcs::ObjectWriteStream stream = client->WriteObject(_bucket, entity.id);
    stream << to_json(entity).dump();
    stream.Close();
    
    StatusOr<gcs::ObjectMetadata> metadata = std::move(stream).metadata();
    if (!metadata) 
    {
        throw std::runtime_error(metadata.status().message());
    }
}

std::optional<Entity> CloudStorageRepo::Retrieve(const std::string& id) const
{
    StatusOr<gcs::Client> client = gcs::Client::CreateDefaultClient();
    if (!client)
    {
        throw std::runtime_error("unable to create cloud storage client");
    }

    gcs::ObjectReadStream stream = client->ReadObject(_bucket, id);
    if(stream.bad())
    {
        return std::nullopt;
    }
    
    Entity e;    
    from_json(json::parse(std::string(std::istreambuf_iterator<char>(stream), {})), e);
    return e;    
}

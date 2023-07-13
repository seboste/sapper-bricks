#include "handler-cli.h"
#include <microservice-essentials/context.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/request/request-processor.h>
#include <microservice-essentials/utilities/metadata-converter.h>
#include <microservice-essentials/utilities/status-converter.h>
#include <CLI/CLI.hpp>
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

void from_json(const json& entity_json, Entity& e)
{    
    entity_json.at("id").get_to(e.id);
    entity_json.at("string_property").get_to(e.string_property);
    entity_json.at("int_property").get_to(e.int_property);
}

}

CliHandler::CliHandler(Api& api, int argc, char **argv)
    : _api(api)
    , _argc(argc)
    , _argv(argv)
{
}

CliHandler::~CliHandler()
{
}

void CliHandler::Handle()
{    
    CLI::App app{ mse::Context::GetGlobalContext().AtOr("app", "UNKNOWN APP") };
    app.require_subcommand(1, 1);

    std::map<std::string, std::string> metadata;
    app.add_option("--metadata,-m", metadata, "metadata key value pairs")->expected(0,-1);
    
    CLI::App* get = app.add_subcommand("get", "gets an entity");
    std::string id;
    get->add_option("--id,1", id, "id of the entity")->required();
    get->callback([&]() { std::cout << getEntity(id, metadata) << std::endl; });

    CLI::App* set = app.add_subcommand("set", "sets an entity");
    std::string data;
    set->add_option("--data,1", data, "the entity as json")->required();
    set->callback([&]() { setEntity(data, metadata); });
    
    try
    {
        app.parse(_argc, _argv);
    } 
    catch(const CLI::ParseError &e)
    {
        app.exit(e);
    }
}

void CliHandler::Stop()
{
}

std::string CliHandler::getEntity(const std::string& id, const std::map<std::string, std::string>& metadata) const
{
    std::string entityJson;
    mse::Status status = mse::RequestHandler("getEntity", mse::Context(mse::ToContextMetadata(metadata)))
        .Process([&](mse::Context&)
        {
            std::optional<Entity> e_optional = _api.GetEntity(id);
            if(!e_optional)
            {
                return mse::Status { mse::StatusCode::not_found, std::string("unknown entity with id " ) + id };
            }

            entityJson = to_json(e_optional.value()).dump();
            return mse::Status::OK;
        });
    if(status.code != mse::StatusCode::ok)
    {
       throw std::runtime_error(status.details.c_str());
    }
    return entityJson;
}

void CliHandler::setEntity(const std::string& data, const std::map<std::string, std::string>& metadata)
{
    mse::Status status = mse::RequestHandler("setEntity", mse::Context(mse::ToContextMetadata(metadata)))
        .Process([&](mse::Context&)
        {
            Entity e;
            try
            {
                from_json(json::parse(data), e);
            }                
            catch(const json::exception& error)
            {
                throw std::invalid_argument(error.what());                
            }
            _api.SetEntity(e);
            return mse::Status::OK;
        });        
    if(status.code != mse::StatusCode::ok)
    {
       throw std::runtime_error(status.details.c_str());
    }
}

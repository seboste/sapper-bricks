#include "handler-http.h"
#include <microservice-essentials/context.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/request/request-processor.h>
#include <microservice-essentials/utilities/metadata-converter.h>
#include <microservice-essentials/utilities/status-converter.h>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <functional>
#include <iostream>
#include <regex>



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

std::string extractId(const std::string& path)
{
    std::smatch ip_result;
    std::regex_match(path, ip_result, std::regex("/(.*)/(.*)"));
    if(ip_result.size() != 3)
    {        
        throw std::invalid_argument("invalid URL");
    }
    
    return ip_result[2];    
}

}

HttpHandler::HttpHandler(Api& api, const std::string& host, int port)
    : _api(api)
    , _svr(std::make_unique<httplib::Server>())
    , _host(host)
    , _port(port)
{        
    _svr->Get("/Entity/(.*)", httplib::Server::Handler(std::bind(&HttpHandler::getEntity, this, std::placeholders::_1, std::placeholders::_2)));
    _svr->Put("/Entity/(.*)", httplib::Server::Handler(std::bind(&HttpHandler::setEntity, this, std::placeholders::_1, std::placeholders::_2)));
}

HttpHandler::~HttpHandler()
{
}

void HttpHandler::Handle()
{    
    MSE_LOG_INFO(std::string("serving on ") + _host + ':' + std::to_string(_port));
    _svr->listen(_host, _port);
}

void HttpHandler::Stop()
{
    MSE_LOG_INFO("stop requested");
    _svr->stop();
}

void HttpHandler::getEntity(const httplib::Request& request, httplib::Response& response)
{
    mse::Status status = mse::RequestHandler("getEntity", mse::Context(mse::ToContextMetadata(request.headers)))
        .Process([&](mse::Context&)
        {
            const std::string id = extractId(request.path);
            std::optional<Entity> e_optional = _api.GetEntity(id);
            if(!e_optional)
            {
                
                return mse::Status { mse::StatusCode::not_found, std::string("unknown entity with id" ) + id };
            }

            response.set_content(
                    to_json(e_optional.value()).dump(),
                    "text/json"
                );
            return mse::Status::OK;
        });
    response.status = mse::ToHttpStatusCode(status.code);
    if(status.code != mse::StatusCode::ok)
    {
        response.body = status.details;
    }
}

void HttpHandler::setEntity(const httplib::Request& request, httplib::Response& response)
{    
    mse::Status status = mse::RequestHandler("setEntity", mse::Context(mse::ToContextMetadata(request.headers)))
        .Process([&](mse::Context&)
        {
            Entity e;
            try
            {
                from_json(json::parse(request.body), e);
            }                
            catch(const json::exception& error)
            {
                throw std::invalid_argument(error.what());                
            }            
            if(e.id != extractId(request.path))
            {
                throw std::invalid_argument("inconsistent id");
            }
            
            _api.SetEntity(e);
            return mse::Status::OK;
        });        
    response.status = mse::ToHttpStatusCode(status.code);
    if(status.code != mse::StatusCode::ok)
    {
        response.body = status.details;
    }
}

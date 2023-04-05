#include "rest-jwks-rsa-key-provider.h"
#include "impl/key_conversion.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT  //be consistent with other projects to prevent seg fault
#include <httplib.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/utilities/url.h>
#include <microservice-essentials/cross-cutting-concerns/error-forwarding-request-hook.h>
#include <exception>
#include <mutex>

namespace
{
std::string requestJWKS(const std::string& jwks_uri)
{
    mse::Url url = mse::ToUrl(jwks_uri);    
    httplib::Client client(url.GetConnectionString());        
    auto resp = client.Get(url.GetWholePath());
    if(resp.error() != httplib::Error::Success || !resp->status)
    {
        const std::string details = resp.error() != httplib::Error::Success
            ? (std::string("requesting JWKS resulted in error: " + httplib::to_string(resp.error())))
            : (std::string("requesting JWKS returned with status ") + std::to_string(resp->status));

        //in case the public keys cannot be retrieved for any reason, this exception allows that the current request 
        //returns the UNAVAILABLE status. To achieve that, each incoming request needs to be wrapped by a ExceptionHandlingRequestHook
        //In the main.cpp, this can be done with the following line:
        //mse::RequestHandler::GloballyWith(mse::ExceptionHandlingRequestHook::Parameters{});    
        throw mse::ErrorForwardingException(mse::Status { mse::StatusCode::unavailable, "" }, details);
    }    
    return resp->body;
}
}

RestJwksRSAKeyProvider::RestJwksRSAKeyProvider(const std::string& jwks_uri, bool throw_on_error)
    : RestJwksRSAKeyProvider([jwks_uri]()->std::string { return requestJWKS(jwks_uri); }) //delegate
{
}

RestJwksRSAKeyProvider::RestJwksRSAKeyProvider(std::function<std::string()> jwks_provider, bool throw_on_error)
    : _jwks_provider(jwks_provider)
    , _throw_on_error(throw_on_error)
    , _key_cache(std::make_shared<Cache>())
{
    updateKeyCache();
}

std::string RestJwksRSAKeyProvider::operator()(const std::string& key_id) const
{
    {
        //1. try with a shared lock for read only access
        std::shared_lock<std::shared_mutex> shared_lock(_key_cache->mutex);        
        if(auto cit = _key_cache->data.find(key_id); cit != _key_cache->data.end())
        {
            return cit->second;
        }
    }

    //not found in cache

    {        
        //2. now try again with an exclusive access (chances are high that it works this time
        //because we only end up here if it's not in the cache. Then maybe some other thread 
        //is updating the cache right now until this thread can enter the critical section exclusively)
        std::unique_lock<std::shared_mutex> exclusive_lock(_key_cache->mutex);        
        if(auto cit = _key_cache->data.find(key_id); cit != _key_cache->data.end())
        {
            return cit->second;
        }

        updateKeyCache();
        
        if(auto cit = _key_cache->data.find(key_id); cit != _key_cache->data.end())
        {
            return cit->second;
        }
    }

    //still not found
    return "";
};

void RestJwksRSAKeyProvider::updateKeyCache() const
{
    try
    {
        const std::string jwks = _jwks_provider();
        const auto keys = jwt_auth_impl::extractPubKeysPEM(jwks);
        if(keys.empty())
        {
            throw std::runtime_error("RestJwksRSAKeyProvider: no keys found");
        }
        _key_cache->data = keys;
    }
    catch(const std::exception& e)
    {        
        if(_throw_on_error) { throw; }
        else { MSE_LOG_ERROR(std::string("unable to obtain JWKS: ") + e.what()); }
    }
    catch(...)
    {
        if(_throw_on_error) { throw; }
        else { MSE_LOG_ERROR("unable to obtain JWKS for unknown reasons."); }
    }
}

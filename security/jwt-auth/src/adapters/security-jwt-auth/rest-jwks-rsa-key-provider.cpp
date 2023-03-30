#include "rest-jwks-rsa-key-provider.h"
#include "impl/key_conversion.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT  //be consistent with other projects to prevent seg fault
#include <httplib.h>
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/utilities/url.h>
#include <microservice-essentials/cross-cutting-concerns/error-forwarding-request-hook.h>
#include <exception>

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
    , _key_cache(std::make_shared<std::map<std::string, std::string>>())
{
    updateKeyCache();
}

std::string RestJwksRSAKeyProvider::operator()(const std::string& key_id) const
{
    auto cit = _key_cache->find(key_id);
    if(cit == _key_cache->end())
    {
        updateKeyCache();
        
        cit = _key_cache->find(key_id);
        if(cit == _key_cache->end())
        {
            return "";
        }
    }
    return cit->second;    
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
        *_key_cache = keys;
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

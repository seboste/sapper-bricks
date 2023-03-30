#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

/**
 * Provides RSA public keys obtained from a jwks_uri.
 * Logs errors or optionally throws a std::exception in case no valid RSA keys can be obtained from the jwks_uri
 * In case receiving the JWKS fails, a mse::ErrorForwardingException is thrown (if desired), allowing a mse::ExceptionHandlingRequestHook to return UNAVAILABLE
 * 
*/
class RestJwksRSAKeyProvider
{
public:    
    RestJwksRSAKeyProvider(const std::string& jwks_uri, bool throw_on_error = true);                //obtains the keys from the jwks uri
    RestJwksRSAKeyProvider(std::function<std::string()> jwks_provider, bool throw_on_error = true); //allows to provide a custom jwks provider (e.g. for testing purposes)    

    //looks for a specific key in the cache. If not present, the _jwks_provider is called. If key is still not available, an empty string is returned.
    //The key is a public RSA key in PEM format.
    //May throw a std::runtime_error in case no keys can be obtained from the uri
    //in case the key_id is empty, an arbitrary key from the cache is returned. Good luck.
    std::string operator()(const std::string& key_id) const;
private:    

    void updateKeyCache() const;

    //share a single cache between multiple copies of this instance (Same jwks_uri => same values => same cache)
    std::shared_ptr<std::map<std::string, std::string>> _key_cache;

    std::function<std::string()> _jwks_provider;
    bool _throw_on_error;
};

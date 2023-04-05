#include "key_conversion.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <jwt/jwt.hpp>
#include <openssl/core_names.h>
#include <openssl/evp.h>
#include <openssl/param_build.h>

std::map<std::string, std::string> jwt_auth_impl::extractPubKeysPEM(const std::string& jwks_json)
{
    std::map<std::string, std::string> keys;
    for(const auto& key : jwt_auth_impl::parse_RS256Keys(jwks_json))
    {
        keys[key.id] = jwt_auth_impl::toPEM_PubKey(key);        
    }
    return keys;
}

std::string jwt_auth_impl::toPEM_PubKey(const RS256Key& key)
{    
    using BIGNUM_ptr = std::unique_ptr<BIGNUM, decltype(&BN_free)>;
    using OSSL_PARAM_BLD_ptr = std::unique_ptr<OSSL_PARAM_BLD, decltype(&OSSL_PARAM_BLD_free)>;
    using OSSL_PARAM_ptr = std::unique_ptr<OSSL_PARAM, decltype(&OSSL_PARAM_free)>;
    using EVP_PKEY_CTX_ptr = std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>;
    using EVP_PKEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;
    
    const std::string dec_modulus = jwt::base64_uri_decode(key.modulus_base64.data(), key.modulus_base64.size());
    const std::string dec_exponent = jwt::base64_uri_decode(key.exponent_base64.data(), key.exponent_base64.size());

    BIGNUM_ptr n(BN_bin2bn((const unsigned char*)dec_modulus.data(), dec_modulus.size(), nullptr), BN_free);
    if(n == nullptr){ throw std::runtime_error("OSSL: unable to convert the modulus to a bignum"); }
    BIGNUM_ptr e(BN_bin2bn((const unsigned char*)dec_exponent.data(), dec_exponent.size(), nullptr), BN_free);
    if(e == nullptr){ throw std::runtime_error("OSSL: unable to convert the exponent to a bignum"); }
    
    OSSL_PARAM_BLD_ptr param_bld(OSSL_PARAM_BLD_new(), OSSL_PARAM_BLD_free);    
    if(param_bld == nullptr
        || !OSSL_PARAM_BLD_push_BN(param_bld.get(), OSSL_PKEY_PARAM_RSA_N, n.release())
        || !OSSL_PARAM_BLD_push_BN(param_bld.get(), OSSL_PKEY_PARAM_RSA_E, e.release())
    ) { throw std::runtime_error("OSSL: unable to init the param builder"); }

    OSSL_PARAM_ptr params(OSSL_PARAM_BLD_to_param(param_bld.get()), OSSL_PARAM_free);    
    if(params == nullptr){ throw std::runtime_error("OSSL: unable to create params"); }
    EVP_PKEY_CTX_ptr ctx(EVP_PKEY_CTX_new_from_name(nullptr, "RSA", nullptr), EVP_PKEY_CTX_free);
    if(ctx == nullptr){ throw std::runtime_error("OSSL: unable to create context"); }

    EVP_PKEY *pub_key_raw = nullptr;
    if(EVP_PKEY_fromdata_init(ctx.get()) <= 0
    || EVP_PKEY_fromdata(ctx.get(), &pub_key_raw, EVP_PKEY_PUBLIC_KEY, params.get()) <= 0
    ) { throw std::runtime_error("OSSL: unable to initialize key"); }
    EVP_PKEY_ptr pub_key(pub_key_raw, EVP_PKEY_free);

    char *buf = nullptr;
    size_t size;
    FILE *stream;
    
    stream = open_memstream (&buf, &size); // POSIX only
    if(stream == nullptr) { throw std::runtime_error("unable to open memory stream"); }
    if(PEM_write_PUBKEY(stream, pub_key.get()) <= 0 || fflush(stream) != 0)
    { 
        fclose(stream);
        free(buf);
        throw std::runtime_error("OSSL: unable write key"); 
    }        
    const std::string result = buf;
    fclose(stream);
    free(buf);

    return result;
}
std::vector<jwt_auth_impl::RS256Key> jwt_auth_impl::parse_RS256Keys(const std::string& jwks_json)
{    
    std::vector<jwt_auth_impl::RS256Key> result;
    json j = json::parse(jwks_json, nullptr, true); //may throw something derived from std::exception
    if(j.contains("keys"))
    {
        for(const auto& key : j["keys"])
        {
            if(!(
                (key.contains("use") && key["use"].is_string() && key["use"] == "sig") ||
                (key.contains("key_ops") && key["key_ops"].is_string() && (key["key_ops"] == "sign" || key["key_ops"] == "verify"))
            ))
            {
                continue;
            }

            
            if(!(key.contains("kty") && key["kty"].is_string() && key["kty"] == "RSA"))
            { 
                continue; 
            }

            if(!(
                (key.contains("kid") && key["kid"].is_string()) &&
                (key.contains("n") && key["n"].is_string()) &&
                (key.contains("e") && key["e"].is_string()) 
            ))
            {
                continue;
            }
            
            jwt_auth_impl::RS256Key k {
                key["kid"],
                key["n"],
                key["e"]
            };
            
            if(k.id.empty() || k.modulus_base64.empty() || k.exponent_base64.empty()) { continue; }
            result.push_back(k);
        }
    }
    return result;
}

#include "jwt-auth-request-hook.h"
#include <microservice-essentials/observability/logger.h>
#include <microservice-essentials/request/request-processor.h>
#include <jwt/jwt.hpp>

namespace 
{

struct TokenImpl
{
    std::string key;
    jwt::jwt_object jwt_object;   
    std::error_code ec; 
};

//see https://www.rfc-editor.org/rfc/rfc7515#section-4.1
struct JWTHeader
{
    std::string alg;
    std::optional<std::string> typ;    
    std::optional<std::string> kid;
};

//extracts the header from an encoded JWT
std::optional<JWTHeader> extract_header(std::string_view enc_token_str)
{
    std::error_code header_ec;
    jwt::jwt_header jwt_header;

    std::string substr = std::string(enc_token_str).substr(0,enc_token_str.find("."));
    jwt_header.decode(substr, header_ec);    
    if(header_ec)
    {        
        return std::nullopt;
    }
    nlohmann::json header_json = jwt_header.create_json_obj();

    JWTHeader header;
    if(!header_json.contains("alg") || !header_json.at("alg").is_string())
    {
        return std::nullopt;
    }
    header.alg = header_json.at("alg").get<std::string>();

    if(header_json.contains("kid") && header_json.at("kid").is_string())
    {
        header.kid = header_json.at("kid").get<std::string>();
    }

    if(header_json.contains("typ") && header_json.at("typ").is_string())
    {
        header.typ = header_json.at("typ").get<std::string>();
    }
    return header;
}

}


JwtAuthRequestHook::Parameters JwtAuthRequestHook::RS256Parameters(const Parameters::KeyProvider& public_key_provider, const std::string& issuer, const std::string& audience, const std::vector<std::string>& required_claims)
{
    Parameters p;
    p.key_provider["RS256"] = public_key_provider;
    p.issuer = issuer;
    p.audience = audience;
    p.required_claims = required_claims;
    return p;
}

JwtAuthRequestHook::Parameters JwtAuthRequestHook::HS256Parameters(const std::string& private_key, const std::string& issuer, const std::string& audience, const std::vector<std::string>& required_claims)
{
    Parameters p;
    p.key_provider["HS256"] = [private_key](const std::string&)->std::string { return private_key;};
    p.issuer = issuer;
    p.audience = audience;
    p.required_claims = required_claims;
    return p;
}

JwtAuthRequestHook::JwtAuthRequestHook(const Parameters& parameters)
    : TokenAuthRequestHook("jwt auth", parameters.metadata_key, parameters.required_claims)
    , _parameters(parameters)
{
}


bool JwtAuthRequestHook::decode_token(const std::string& token, std::any& decoded_token, std::string& decoding_details) const
{
    decoding_details = "";

    //strip off the prefix
    if(token.substr(0, _parameters.prefix.size()) != _parameters.prefix)
    {
        decoding_details = "invalid token prefix";
        return false;
    }
    const std::string enc_str = token.substr(_parameters.prefix.size());
    
    //decode the token   
    std::optional<JWTHeader> header_opt = extract_header(enc_str);
    if(!header_opt.has_value())
    {
        decoding_details = "invalid token header";
        return false;
    }
    JWTHeader header = header_opt.value();

    if(!header.typ.has_value() && header.typ.value() != "JWT")
    {
        decoding_details = "invalid token type";
        return false;
    }

    const auto key_provider_cit = _parameters.key_provider.find(header.alg);
    if(key_provider_cit == _parameters.key_provider.end())
    {
        decoding_details = "no key provider available";
        return false;
    }
    const std::string algorithm = key_provider_cit->first;
    const std::string key = key_provider_cit->second(header.kid.value_or(""));
    if(key.empty())
    {
        decoding_details = "key not found";
        return false;
    }

    TokenImpl token_impl;
    token_impl.key = key;
    using namespace jwt::params;
    token_impl.jwt_object = jwt::decode(enc_str, 
        algorithms({algorithm.c_str()}),
        token_impl.ec, 
        secret(token_impl.key), 
        issuer(_parameters.issuer),
        aud(_parameters.audience),
        verify(true)    //verification should actually be done later in verify_token. This does not seem to be possible with that lib though.
        );

    
    if(token_impl.ec && (token_impl.ec.category() == jwt::theAlgorithmErrCategory || token_impl.ec.category() == jwt::theDecodeErrorCategory))
    {
        decoding_details = token_impl.ec.message();
        return false;
    }

    decoded_token = token_impl;
    return true;
}

bool JwtAuthRequestHook::verify_token(const std::any& decoded_token, std::string& verification_details) const
{
    verification_details = "";
    const TokenImpl& token_impl = std::any_cast<const TokenImpl&>(decoded_token);
    if(token_impl.ec)
    {
        verification_details = token_impl.ec.message();
        return false;
    }
    return true;
}

std::optional<std::string> JwtAuthRequestHook::extract_claim(const std::any& decoded_token, const std::string& claim) const
{
    const TokenImpl& token = std::any_cast<const TokenImpl&>(decoded_token);    
    if(!token.jwt_object.payload().has_claim(claim))
    {
        return std::nullopt;
    }
    return token.jwt_object.payload().get_claim_value<std::string>(claim);    
}

#pragma once
#include <microservice-essentials/security/token-auth-request-hook.h>
#include <microservice-essentials/request/request-hook-factory.h>

#include <functional>
#include <optional>
#include <map>


/** 
*/
class JwtAuthRequestHook : public mse::TokenAuthRequestHook
{
public:    

    struct Parameters
    {
        typedef std::function<std::string(const std::string& key_id)> KeyProvider; //key_id gives a hint, which key to return. It can be empty. Needs to return an empty string if the requested key is not available.
        std::map<std::string, KeyProvider> key_provider;    //maps an algorithm (e.g. HS256, RS256) to a provider for the required key. See https://github.com/arun11299/cpp-jwt for a list of supported algorithms.
        std::string issuer = "";                            //token's 'iss' claim needs to match this string. Can be found in the openid configuration.
        std::string audience = "";                          //token's 'aud' claim needs to match this string. Defined by the application.
        std::vector<std::string> required_claims;           //claims that must be available in the token. They will be automatically be stored in the request's context.
                                                            //A use case for this would be authorization based on scopes. The scope needs to be transferred in to the context 
                                                            //(done by 'required_claims = {"scope"}') and each request handler would check the scope 
                                                            //(e.g. by simply adding '.With(mse::ClaimCheckerRequestHook::ScopeContains("read:entity"))' to the request getEntity request handler)
        std::string metadata_key = "authorization";         //in which metadata key of the context can the token be found?
        std::string prefix = "Bearer ";                     //the prefix of the token

        mse::AutoRequestHookParameterRegistration<JwtAuthRequestHook::Parameters, JwtAuthRequestHook> auto_registration;
    };

    static Parameters RS256Parameters(const Parameters::KeyProvider& public_key_provider, const std::string& issuer, const std::string& audience, const std::vector<std::string>& required_claims = {} );
    static Parameters HS256Parameters(const std::string& private_key, const std::string& issuer, const std::string& audience, const std::vector<std::string>& required_claims = {} );


    JwtAuthRequestHook(const Parameters& parameters);
    virtual ~JwtAuthRequestHook() = default;

protected:

    virtual bool decode_token(const std::string& token, std::any& decoded_token, std::string& decoding_details) const override;
    virtual bool verify_token(const std::any& decoded_token, std::string& verification_details) const override;
    virtual std::optional<std::string> extract_claim(const std::any& decoded_token, const std::string& claim) const override;

private:
    Parameters _parameters;
};


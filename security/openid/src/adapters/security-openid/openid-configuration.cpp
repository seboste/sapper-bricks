#include "openid-configuration.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT  //be consistent with other projects to prevent seg fault
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <microservice-essentials/utilities/url.h>
#include <exception>

using namespace OpenID;

namespace {

std::optional<std::string> extractString(const nlohmann::json& json, const std::string& json_key)
{
    if(!json.contains(json_key) || !json.at(json_key).is_string())
    {
        return std::nullopt;
    }
    return json.at(json_key).get<std::string>();
}

std::optional<bool> extractBool(const nlohmann::json& json, const std::string& json_key)
{
      if(!json.contains(json_key) || !json.at(json_key).is_boolean())
    {
        return std::nullopt;
    }
    return json.at(json_key).get<bool>();
}

std::optional<std::vector<std::string>> extractStrings(const nlohmann::json& json, const std::string& json_key)
{
    if(!json.contains(json_key) || !json.at(json_key).is_array())
    {
        return std::nullopt;
    }

    std::vector<std::string> values;
    for(const auto& cit : json.at(json_key))
    {
        if(cit.is_string())
        {
            values.push_back(cit.get<std::string>());
        }
    }
    return values;
}

}

Configuration OpenID::CreateFromJson(const std::string& openid_config_json)
{
    const nlohmann::json json = nlohmann::json::parse(openid_config_json);

    Configuration config;
    config.issuer = extractString(json, "issuer").value();
    config.authorization_endpoint = extractString(json, "authorization_endpoint").value();
    config.token_endpoint = extractString(json, "token_endpoint");
    config.userinfo_endpoint = extractString(json, "userinfo_endpoint");
    config.jwks_uri = extractString(json, "jwks_uri").value();
    config.registration_endpoint = extractString(json, "registration_endpoint");
    config.scopes_supported = extractStrings(json, "scopes_supported");
    config.response_types_supported = extractStrings(json, "response_types_supported").value();
    config.response_modes_supported = extractStrings(json, "response_modes_supported");
    config.grant_types_supported = extractStrings(json, "grant_types_supported");
    config.acr_values_supported = extractStrings(json, "acr_values_supported");
    config.subject_types_supported = extractStrings(json, "subject_types_supported").value();
    config.id_token_signing_alg_values_supported = extractStrings(json, "id_token_signing_alg_values_supported").value();
    config.id_token_encryption_alg_values_supported = extractStrings(json, "id_token_encryption_alg_values_supported");
    config.id_token_encryption_enc_values_supported = extractStrings(json, "id_token_encryption_enc_values_supported");
    config.userinfo_signing_alg_values_supported = extractStrings(json, "userinfo_signing_alg_values_supported");
    config.userinfo_encryption_alg_values_supported = extractStrings(json, "userinfo_encryption_alg_values_supported");
    config.userinfo_encryption_enc_values_supported = extractStrings(json, "userinfo_encryption_enc_values_supported");
    config.request_object_signing_alg_values_supported = extractStrings(json, "request_object_signing_alg_values_supported");
    config.request_object_encryption_alg_values_supported = extractStrings(json, "request_object_encryption_alg_values_supported");
    config.request_object_encryption_enc_values_supported = extractStrings(json, "request_object_encryption_enc_values_supported");
    config.token_endpoint_auth_methods_supported = extractStrings(json, "token_endpoint_auth_methods_supported");
    config.token_endpoint_auth_signing_alg_values_supported = extractStrings(json, "token_endpoint_auth_signing_alg_values_supported");
    config.display_values_supported = extractStrings(json, "display_values_supported");
    config.claim_types_supported = extractStrings(json, "claim_types_supported");
    config.claims_supported = extractStrings(json, "claims_supported");
    config.service_documentation = extractString(json, "service_documentation");
    config.claims_locales_supported = extractStrings(json, "claims_locales_supported");
    config.ui_locales_supported = extractStrings(json, "ui_locales_supported");
    config.claims_parameter_supported = extractBool(json, "claims_parameter_supported");
    config.request_parameter_supported = extractBool(json, "request_parameter_supported");
    config.request_uri_parameter_supported = extractBool(json, "request_uri_parameter_supported");
    config.require_request_uri_registration = extractBool(json, "require_request_uri_registration");
    config.op_policy_uri = extractString(json, "op_policy_uri");
    config.op_tos_uri = extractString(json, "op_tos_uri");

    return config;
}

Configuration OpenID::CreateFromUrl(const std::string& openid_config_url)
{
    mse::Url url = mse::ToUrl(openid_config_url);
    httplib::Client client(url.GetConnectionString());
    client.set_connection_timeout(3, 0);
    client.set_read_timeout(3, 0);
    auto resp = client.Get(url.GetWholePath());
    if(resp.error() != httplib::Error::Success)
    {
        throw std::runtime_error(std::string("requesting openid configuration resulted in error: ") + httplib::to_string(resp.error()));
    }
    if(!resp->status)
    {
        throw std::runtime_error(std::string("receiving openid configuration returned with status ") + std::to_string(resp->status));
    }
    return CreateFromJson(resp->body);
}

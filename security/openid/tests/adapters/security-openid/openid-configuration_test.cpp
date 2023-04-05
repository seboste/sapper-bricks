#include <catch2/catch_test_macros.hpp>
#include <adapters/security-openid/openid-configuration.h>
#include <iostream>


SCENARIO( "OpenID Configuration", "[security][openid]" )
{
  GIVEN("a json open id config from auth0")
  {
    const std::string json = R""(
    {
      "issuer": "https://dev-abcdef.us.auth0.com/",
      "authorization_endpoint": "https://dev-abcdef.us.auth0.com/authorize",
      "token_endpoint": "https://dev-abcdef.us.auth0.com/oauth/token",
      "device_authorization_endpoint": "https://dev-abcdef.us.auth0.com/oauth/device/code",
      "userinfo_endpoint": "https://dev-abcdef.us.auth0.com/userinfo",
      "mfa_challenge_endpoint": "https://dev-abcdef.us.auth0.com/mfa/challenge",
      "jwks_uri": "https://dev-abcdef.us.auth0.com/.well-known/jwks.json",
      "registration_endpoint": "https://dev-abcdef.us.auth0.com/oidc/register",
      "revocation_endpoint": "https://dev-abcdef.us.auth0.com/oauth/revoke",
      "scopes_supported": ["openid","profile","offline_access","name","given_name","family_name","nickname","email","email_verified","picture","created_at","identities","phone","address"],
      "response_types_supported": ["code","token","id_token","code token","code id_token","token id_token","code token id_token"],
      "code_challenge_methods_supported": ["S256","plain"],
      "response_modes_supported": ["query","fragment","form_post"],
      "subject_types_supported": ["public"],
      "id_token_signing_alg_values_supported": ["HS256","RS256"],
      "token_endpoint_auth_methods_supported": ["client_secret_basic","client_secret_post"],
      "claims_supported": ["aud","auth_time","created_at","email","email_verified","exp","family_name","given_name","iat","identities","iss","name","nickname","phone_number","picture","sub"],
      "request_uri_parameter_supported": false,
      "request_parameter_supported": false
    }
    )"";
    WHEN("a configuration is created from that json")
    {
      const OpenID::Configuration config = OpenID::CreateFromJson(json);
      THEN("all required values are set correctly")
      {
          REQUIRE(config.issuer == "https://dev-abcdef.us.auth0.com/");
          REQUIRE(config.authorization_endpoint == "https://dev-abcdef.us.auth0.com/authorize");
          REQUIRE(config.jwks_uri == "https://dev-abcdef.us.auth0.com/.well-known/jwks.json");   
          REQUIRE(config.response_types_supported == std::vector<std::string>{"code","token","id_token","code token","code id_token","token id_token","code token id_token"});
          REQUIRE(config.subject_types_supported == std::vector<std::string>{"public"});    
          REQUIRE(config.id_token_signing_alg_values_supported == std::vector<std::string>{"HS256","RS256"});
      }

      AND_THEN("some optional values are also set correctly")
      {
        REQUIRE(config.response_modes_supported.value() == std::vector<std::string>{"query","fragment","form_post"});
        REQUIRE(config.request_parameter_supported.value() == false);
        REQUIRE(config.token_endpoint.value() == "https://dev-abcdef.us.auth0.com/oauth/token");
      }
      
      AND_THEN("some unset values are nullopt")
      {
        REQUIRE(config.service_documentation != std::nullopt);
      }
    }
  }
  GIVEN("a json with invalid syntax")
  {
    const std::string json = R""({"issuer": "https://dev-abcd'])"";
    WHEN("a configuration is created from that json")
    {
      THEN("an exception of type std::exception is thrown")
      {
        REQUIRE_THROWS_AS(OpenID::CreateFromJson(json), std::exception);
      }
    }
  }
  GIVEN("a valid json with missing required field")
  {   
    //missing: "issuer": "https://dev-abcdef.us.auth0.com/",
    const std::string json =R""(
      {
        "authorization_endpoint": "https://dev-abcdef.us.auth0.com/authorize",
        "jwks_uri": "https://dev-abcdef.us.auth0.com/.well-known/jwks.json",
        "response_types_supported": ["code","token","id_token","code token","code id_token","token id_token","code token id_token"],
        "subject_types_supported": ["public"],
        "id_token_signing_alg_values_supported": ["HS256","RS256"]
      }
      )"";
    WHEN("a configuration is created from that json")
    {
      THEN("an exception of type std::bad_optional_access is thrown")
      {
        REQUIRE_THROWS_AS(OpenID::CreateFromJson(json), std::bad_optional_access);
      }
    }
  }
}
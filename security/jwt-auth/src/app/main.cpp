//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/security-jwt-auth/jwt-auth-request-hook.h>
#include <adapters/security-jwt-auth/rest-jwks-rsa-key-provider.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
        ...
    
        //OpenID::Configuration openid_config = OpenID::CreateFromUrl(mse::getenv_or("OPENID_URL","<<<OPENID_DEFAULT_URL>>>"));
//<<<SAPPER SECTION BEGIN APPEND MAIN-INITIALIZATION>>>        
        RestJwksRSAKeyProvider key_provider(openid_config.jwks_uri);
//<<<SAPPER SECTION END APPEND MAIN-INITIALIZATION>>>

//<<<SAPPER SECTION BEGIN APPEND MAIN-REQUEST-HANDLER-HOOKS>>>
        mse::RequestHandler::GloballyWith(JwtAuthRequestHook::RS256Parameters(key_provider, openid_config.issuer, "<<<JWT_AUTH_AUDIENCE>>>", <<<JWT_AUTH_REQUIRED_CLAIMS>>>));
//<<<SAPPER SECTION END APPEND MAIN-REQUEST-HANDLER-HOOKS>>>
        ...
}

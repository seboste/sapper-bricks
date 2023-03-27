//<<<SAPPER SECTION BEGIN APPEND MAIN-INCLUDES>>>
#include <adapters/security-openid/openid-configuration.h>
//<<<SAPPER SECTION END APPEND MAIN-INCLUDES>>>

int main()
{
    ...
    
//<<<SAPPER SECTION BEGIN APPEND MAIN-INITIALIZATION>>>
    OpenID::Configuration openid_config = OpenID::CreateFromUrl(mse::getenv_or("OPENID_URL","<<<OPENID_DEFAULT_URL>>>"));    
//<<<SAPPER SECTION END APPEND MAIN-INITIALIZATION>>>

    ...
}

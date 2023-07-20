# security-openid

This helper brick provides an [openid configuration](https://openid.net/specs/openid-connect-discovery-1_0.html#ProviderConfig) to your service. Amongst others, this configuration provides the *jwks_uri* that is required for [JWT](https://jwt.io/) based authentication.

This brick uses the **cpp-httplib** library for issuing that http request and the **nlohmann_json** for parsing the openid configuration JSON.

## usage

Choose an [identity provider](https://www.google.com/search?q=list+of+identity+provider), make yourself familiar with it, create an account and if applicaple a project. Then obtain the openid-url.

## parameters

-  **OPENID_DEFAULT_URL**: The URL from which the openid-configuration is retrieved as JSON. The URL needs to be obtained from your identity provider. It typically follows the `https://www.example.com/.well-known/openid-configuration` pattern.

## next steps

Use the openid-configuration by e.g. the *security-jwt-auth* brick.

## open issues

As most of the code provided by this brick is and probably remains identical for all your microservices, the code should be moved to a library. This would facilitate the maintenance.
 
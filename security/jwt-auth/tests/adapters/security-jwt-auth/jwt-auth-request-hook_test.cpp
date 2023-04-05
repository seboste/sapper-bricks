#include <catch2/catch_test_macros.hpp>
#include <adapters/security-jwt-auth/jwt-auth-request-hook.h>


namespace {

/*
{
  "alg": "RS256",
  "typ": "JWT",
  "kid" : "key0"
}
{
  "iss" : "the_issuer",
  "aud" : "the_audience",
  "exp" : 9999999999,  
  "scope": "my_scope",
  "iat": 1516239022
}
*/

const std::string valid_token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImtleTAifQ.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiYXVkIjoidGhlX2F1ZGllbmNlIiwiZXhwIjo5OTk5OTk5OTk5LCJzY29wZSI6Im15X3Njb3BlIiwiaWF0IjoxNTE2MjM5MDIyfQ.RJespS2tdKtrmo-zcpfU3lpszljiF6hacXEwlfpkGz1wafyUt-sOHIVQyPtOjir4UoHrllkst4MQuNm4FfXxgorYmVxxuN29q0T2zv0kiUgbj4pBBf7ANfmc2WRi2nIrPa4UxpzskIjuQW4HY2QO9qXAUsQdRLvEwwd0aHv3hRmskIsyamzqxeEvAIxpnnSpDPpQd7cZGCqZ3oJ-D2sx1aYggEYk8IBs4LA8olLRur8x6sNfvXo5B852ekALnyqDAgXxdKX8pyVIYQYSQ1HUTZEppsI77HmTxMEW5_gK6TOJw7M2G2KZB8hFiPlulpYeeOZaXx3TSqnsVaCLsZlTjg";
const std::string invalid_sig_token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImtleTAifQ.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiYXVkIjoidGhlX2F1ZGllbmNlIiwiZXhwIjo5OTk5OTk5OTk5LCJzY29wZSI6Im15X3Njb3BlIiwiaWF0IjoxNTE2MjM5MDIyfQ.RJespS2tdKtrmo-zcpfU3lpszljiF6hacXEwlfpkGz1wafyUt-sOHIVQyPtOjir4UoHrllkst4MQuNm4FfXxgorYmVxxuN29q0T2zv0kiUgbj4pBBf7ANfmc2WRi2nIrPa4UxpzskIjuQW4HY2QO9qXAUsQdRLvEwwd0aHv3hRmskIsyamzqxeEvAIxpnnSpDPpQd7cZGCqZ3oJ-D2sx1aYggEYk8IBs4LA8olLRur8x6sNfvXo5B852ekALnyqDAgXxdKX8pyVIYQYSQ1HUTZEppsI77HmTxMEW5_gK6TOJw7M2G2KZB8hFiPlulpYeeOZaXx3TS_THIS_IS_INVALID_qnsVaCLsZlTjg";
const std::string no_scope_token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImtleTAifQ.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiYXVkIjoidGhlX2F1ZGllbmNlIiwiZXhwIjo5OTk5OTk5OTk5LCJpYXQiOjE1MTYyMzkwMjJ9.MtERei_vZJ9yGtEm3Kf1T5VomNwf1u47ksTrNX7Z1aYEj9Qj_07A6t8PiBzd-3R8nHnYZKpMH__rfk5tTAekBJGfcnES-6i5f2jZW8NkjJypTrOrFRjej8Q3-QUIhZx7HOrJ_HsKi1thUU0m1tgMdC7psqR1dieTYXitz6NoFePG2EJF5zJ6o6UAQcriupPQ4LYLW4l1d4Y1tVcjmcuzWtf3e7jnDhz6O8vJwYHmo7xaDaCGdi8EZLbfUiWZyH854pdrQOiEY-Kku77C_VcnhnuU-LBB0wLALPfVn-ArvvDW6CpldIouasKwv36kAd7sXR8J5SeT6SRsOAqTFO3HXw";
const std::string expired_token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImtleTAifQ.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiYXVkIjoidGhlX2F1ZGllbmNlIiwiZXhwIjoxNTE2MjM5MDIyLCJzY29wZSI6Im15X3Njb3BlIiwiaWF0IjoxNTE2MjM5MDIyfQ.bn5WvhmCHjDjYX75i0B0jGqsfSD7xt5USIu2Hhb2Fzv2XT-3jpIxxj9P1_kGpq9NslJaGLwNUZHqPjpFcWAzvJMj5F7F6XRX8poY_a2PHjpCTV1xzkyaJ-N2x5QFWUS5rOiIrYzEU-cxcWbeZXXC9O6rqEJpKHojixFGGq1FbGZJW39gFcKqptf-6uxPiTgGEef48NRjE4NdnVGj5OTFquqC-jHQ5WhqF8EvbkccCWyP4Uv1J1dZVunpyDYB2853Q2N42TntwBTzhyomXjolp9SMMWnkHa_17aKBVXxJpwXBp8HdgiUmO_VOlPs6gR3hUTyysDmyIreGEZ6nH1mdhw";
const std::string no_audience_token = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6ImtleTAifQ.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiZXhwIjo5OTk5OTk5OTk5LCJzY29wZSI6Im15X3Njb3BlIiwiaWF0IjoxNTE2MjM5MDIyfQ.aNQRAmZEhXmVquuJ5Rzltdb-1aI_BKwNkEa7UqG-WCB2fFLANJR2V9EW_ZN8Zjq9AQcmTcQFE2fwLyyS0jdTHOoE3gy1sMqh4DdjZ44JG_iNfqNnO6tphYmYCvHVipviL0zdpRnHnE5lfW-fwNLNZqgYOfxOMJX8Kgd2IV_UgCP372JGQFM5UTEC_nZ5kFTDEYvcUxfRPLTDr_Cn_m4zrShLbtabWeIEMUQ5BTfwKH8bMdCqMluOvbRHzujbg3nK38ogQ_tI62Q5epGdRAjV60UF01lVrqBFWmMcP-H7COEdqZuFpi7jnFRlXCheKRxJV3z6E7rgzHFg8xYFzQIglg";

const std::string public_key = R"(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAu1SU1LfVLPHCozMxH2Mo
4lgOEePzNm0tRgeLezV6ffAt0gunVTLw7onLRnrq0/IzW7yWR7QkrmBL7jTKEn5u
+qKhbwKfBstIs+bMY2Zkp18gnTxKLxoS2tFczGkPLPgizskuemMghRniWaoLcyeh
kd3qqGElvW/VDL5AaWTg0nLVkjRo9z+40RQzuVaE8AkAFmxZzow3x+VJYKdjykkJ
0iT9wCS0DRTXu269V264Vf/3jvredZiKRkgwlL9xNAwxXFg0x/XFw005UWVRIkdg
cKWTjpBP2dPwVZ4WWC+9aGVd+Gyn1o0CLelf4rEjGoXbAAEgAqeGUxrcIlbjXfbc
mwIDAQAB
-----END PUBLIC KEY-----
)";

/* private key is just required to recreate the JWT
const std::string private_key = R"(
-----BEGIN PRIVATE KEY-----
MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQC7VJTUt9Us8cKj
MzEfYyjiWA4R4/M2bS1GB4t7NXp98C3SC6dVMvDuictGeurT8jNbvJZHtCSuYEvu
NMoSfm76oqFvAp8Gy0iz5sxjZmSnXyCdPEovGhLa0VzMaQ8s+CLOyS56YyCFGeJZ
qgtzJ6GR3eqoYSW9b9UMvkBpZODSctWSNGj3P7jRFDO5VoTwCQAWbFnOjDfH5Ulg
p2PKSQnSJP3AJLQNFNe7br1XbrhV//eO+t51mIpGSDCUv3E0DDFcWDTH9cXDTTlR
ZVEiR2BwpZOOkE/Z0/BVnhZYL71oZV34bKfWjQIt6V/isSMahdsAASACp4ZTGtwi
VuNd9tybAgMBAAECggEBAKTmjaS6tkK8BlPXClTQ2vpz/N6uxDeS35mXpqasqskV
laAidgg/sWqpjXDbXr93otIMLlWsM+X0CqMDgSXKejLS2jx4GDjI1ZTXg++0AMJ8
sJ74pWzVDOfmCEQ/7wXs3+cbnXhKriO8Z036q92Qc1+N87SI38nkGa0ABH9CN83H
mQqt4fB7UdHzuIRe/me2PGhIq5ZBzj6h3BpoPGzEP+x3l9YmK8t/1cN0pqI+dQwY
dgfGjackLu/2qH80MCF7IyQaseZUOJyKrCLtSD/Iixv/hzDEUPfOCjFDgTpzf3cw
ta8+oE4wHCo1iI1/4TlPkwmXx4qSXtmw4aQPz7IDQvECgYEA8KNThCO2gsC2I9PQ
DM/8Cw0O983WCDY+oi+7JPiNAJwv5DYBqEZB1QYdj06YD16XlC/HAZMsMku1na2T
N0driwenQQWzoev3g2S7gRDoS/FCJSI3jJ+kjgtaA7Qmzlgk1TxODN+G1H91HW7t
0l7VnL27IWyYo2qRRK3jzxqUiPUCgYEAx0oQs2reBQGMVZnApD1jeq7n4MvNLcPv
t8b/eU9iUv6Y4Mj0Suo/AU8lYZXm8ubbqAlwz2VSVunD2tOplHyMUrtCtObAfVDU
AhCndKaA9gApgfb3xw1IKbuQ1u4IF1FJl3VtumfQn//LiH1B3rXhcdyo3/vIttEk
48RakUKClU8CgYEAzV7W3COOlDDcQd935DdtKBFRAPRPAlspQUnzMi5eSHMD/ISL
DY5IiQHbIH83D4bvXq0X7qQoSBSNP7Dvv3HYuqMhf0DaegrlBuJllFVVq9qPVRnK
xt1Il2HgxOBvbhOT+9in1BzA+YJ99UzC85O0Qz06A+CmtHEy4aZ2kj5hHjECgYEA
mNS4+A8Fkss8Js1RieK2LniBxMgmYml3pfVLKGnzmng7H2+cwPLhPIzIuwytXywh
2bzbsYEfYx3EoEVgMEpPhoarQnYPukrJO4gwE2o5Te6T5mJSZGlQJQj9q4ZB2Dfz
et6INsK0oG8XVGXSpQvQh3RUYekCZQkBBFcpqWpbIEsCgYAnM3DQf3FJoSnXaMhr
VBIovic5l0xFkEHskAjFTevO86Fsz1C2aSeRKSqGFoOQ0tmJzBEs1R6KqnHInicD
TQrKhArgLXX4v3CddjfTRJkFWDbE/CkvKZNOrcf1nhaGCPspRJj2KUkj1Fhl9Cnc
dn/RsYEONbwQSjIfMPkvxF+8HQ==
-----END PRIVATE KEY-----
)";
*/

const std::string hs256_key = "FB685C66CE0FDBAF8D57C86D86F94448B9369C423E47A91F69A0CB0AF5891F02";
const std::string hs256_valid_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiYXVkIjoidGhlX2F1ZGllbmNlIiwiZXhwIjo5OTk5OTk5OTk5LCJzY29wZSI6Im15X3Njb3BlIiwiaWF0IjoxNTE2MjM5MDIyfQ.b7PgRvh71EsWIWPDZ8mc_iaEjygucSPrcUz8wYEDrx8";
const std::string hs256_invalid_token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJ0aGVfaXNzdWVyIiwiYXVkIjoidGhlX2F1ZGllbmNlIiwiZXhwIjo5OTk5OTk5OTk5LCJzY29wZSI6Im15X3Njb3BlIiwiaWF0IjoxNTE2MjM5MDIyfQ.b7PgRvh71EsWIWPDZ8mc_iaEjygucSPrcUz8wYEDrx9";

} //end anon NS


SCENARIO( "JWT Auth Request Hook", "[security][jwt auth]" )
{
    GIVEN("a JWT Auth Request Hook based on RS256 Parameters with hardcoded public key")
    {
        JwtAuthRequestHook::Parameters parameters;
        parameters.key_provider["RS256"] = [=](const std::string&) { return public_key; };
        parameters.issuer = "the_issuer";
        parameters.audience = "the_audience";
        parameters.required_claims.push_back("scope");
        
        JwtAuthRequestHook hook(parameters);

        WHEN("the hook processes a request with a context containing a valid jwt")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + valid_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("OK is returned")
            {
                REQUIRE(status == mse::Status::OK);
            }
            AND_THEN("the scope has been written to the context")
            {
                REQUIRE(context.AtOr("scope", "invalid") == "my_scope");
            }            
        }

        WHEN("the hook processes a request with a context containing a valid jwt and a scope")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + valid_token);
            context.Insert("scope", "previously set scope");
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("OK is returned")
            {
                REQUIRE(status == mse::Status::OK);
            }
            AND_THEN("the scope in the context has been overwritten")
            {
                REQUIRE(context.AtOr("scope", "invalid") == "my_scope");
            }            
        }

        WHEN("the hook processes a request with a context containing a jwt with an invalid signature")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + invalid_sig_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("UNAUTHENTICATED is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }
        }

        WHEN("the hook processes a request with a context without jwt")
        {
            mse::Context context;            
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("UNAUTHENTICATED is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }
        }

        WHEN("the hook processes a request with a context containing a jwt without audience")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + no_audience_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("UNAUTHENTICATED is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }
        }

        WHEN("the hook processes a request with a context containing an exprired jwt")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + expired_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("UNAUTHENTICATED is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }
        }
    }

    GIVEN("a JWT Auth Request Hook based on RS256 Parameters with hardcoded public key and wrong issuer")
    {
        JwtAuthRequestHook::Parameters parameters;
        parameters.key_provider["RS256"] = [=](const std::string&) { return public_key; };
        parameters.issuer = "wrong_issuer";
        parameters.audience = "the_audience";
        parameters.required_claims.push_back("scope");
        
        JwtAuthRequestHook hook(parameters);

        WHEN("the hook processes a request with a context containing a valid jwt")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + valid_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("UNAUTHENTICATED is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }          
        }
    }

    GIVEN("a JWT Auth Request Hook based on RS256 Parameters with hardcoded public key and wrong audience")
    {
        JwtAuthRequestHook::Parameters parameters;
        parameters.key_provider["RS256"] = [=](const std::string&) { return public_key; };
        parameters.issuer = "the_issuer";
        parameters.audience = "wrong_audience";
        parameters.required_claims.push_back("scope");
        
        JwtAuthRequestHook hook(parameters);

        WHEN("the hook processes a request with a context containing a valid jwt")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + valid_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);
            THEN("UNAUTHENTICATED is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }          
        }
    }

    GIVEN("a JWT Auth Request Hook based on HS256 Parameters")
    {
        JwtAuthRequestHook hook(JwtAuthRequestHook::HS256Parameters(hs256_key, "the_issuer", "the_audience"));

        WHEN("the hook processes a request with a context containing a valid jwt")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + hs256_valid_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);            
            THEN("OK is returned")
            {
                REQUIRE(status == mse::Status::OK);
            }
        }

         WHEN("the hook processes a request with a context containing a jwt with an invalid signature")
        {
            mse::Context context;
            context.Insert("authorization", std::string("Bearer ") + hs256_invalid_token);
            const mse::Status status = hook.Process([](mse::Context&) { return mse::Status::OK;}, context);            
            THEN("unauthenticated is returned")
            {
                REQUIRE(status.code == mse::StatusCode::unauthenticated);
            }            
        }
    }
}
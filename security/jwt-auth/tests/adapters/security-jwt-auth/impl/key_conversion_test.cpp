#include <catch2/catch_test_macros.hpp>
#include <adapters/security-jwt-auth/impl/key_conversion.h>

SCENARIO( "Key Conversion", "[security][jwt auth]" )
{
    GIVEN("a key modulus and exponent")
    {
        //example taken from https://8gwifi.org/jwkconvertfunctions.jsp
        const jwt_auth_impl::RS256Key key = 
        {
            "test",
            "33TqqLR3eeUmDtHS89qF3p4MP7Wfqt2Zjj3lZjLjjCGDvwr9cJNlNDiuKboODgUiT4ZdPWbOiMAfDcDzlOxA04DDnEFGAf-kDQiNSe2ZtqC7bnIc8-KSG_qOGQIVaay4Ucr6ovDkykO5Hxn7OU7sJp9TP9H0JH8zMQA6YzijYH9LsupTerrY3U6zyihVEDXXOv08vBHk50BMFJbE9iwFwnxCsU5-UZUZYw87Uu0n4LPFS9BT8tUIvAfnRXIEWCha3KbFWmdZQZlyrFw0buUEf0YN3_Q0auBkdbDR_ES2PbgKTJdkjc_rEeM0TxvOUf7HuUNOhrtAVEN1D5uuxE1WSw",
            "AQAB"
        };

        const std::string expectedPEM = R"(-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA33TqqLR3eeUmDtHS89qF
3p4MP7Wfqt2Zjj3lZjLjjCGDvwr9cJNlNDiuKboODgUiT4ZdPWbOiMAfDcDzlOxA
04DDnEFGAf+kDQiNSe2ZtqC7bnIc8+KSG/qOGQIVaay4Ucr6ovDkykO5Hxn7OU7s
Jp9TP9H0JH8zMQA6YzijYH9LsupTerrY3U6zyihVEDXXOv08vBHk50BMFJbE9iwF
wnxCsU5+UZUZYw87Uu0n4LPFS9BT8tUIvAfnRXIEWCha3KbFWmdZQZlyrFw0buUE
f0YN3/Q0auBkdbDR/ES2PbgKTJdkjc/rEeM0TxvOUf7HuUNOhrtAVEN1D5uuxE1W
SwIDAQAB
-----END PUBLIC KEY-----
)";

        WHEN("the key is converted into the PEM format")
        {
            const std::string pem = jwt_auth_impl::toPEM_PubKey(key);
            THEN("the converted key is as expected")
            {
                REQUIRE(pem == expectedPEM);
            }
        }
    }

    GIVEN("a jwks with two RSA signature keys, 1 key with different key type, and 1 non signature key")
    {
        const std::string jwks = R"({"keys": [
            {
                "kty": "RSA",
                "use": "sig",
                "n": "abc",
                "e": "def",
                "kid": "key_0"
            },
            {
                "kty": "RSA",
                "key_ops": "sign",
                "n": "xyz",
                "e": "123",
                "kid": "key_1"
            },
            {
                "kty": "EC",
                "use": "sig",
                "n": "aaa",
                "e": "bbb",
                "kid": "key_2"
            },
            {
                "kty": "RSA",
                "use": "enc",
                "n": "aaa",
                "e": "bbb",
                "kid": "key_3"
            }
            ]})";
        WHEN("the jwks is parsed")
        {
            const std::vector<jwt_auth_impl::RS256Key> keys = jwt_auth_impl::parse_RS256Keys(jwks);
            THEN("the two RSA keys are available")
            {
                REQUIRE(keys.size() == 2);
                REQUIRE(keys[0].id == "key_0");
                REQUIRE(keys[0].modulus_base64 == "abc");
                REQUIRE(keys[0].exponent_base64 == "def");                
                REQUIRE(keys[1].id == "key_1");
                REQUIRE(keys[1].modulus_base64 == "xyz");
                REQUIRE(keys[1].exponent_base64 == "123");
            }
        }
    }

    GIVEN("a jwks with invalid syntax")
    {
        const std::string invalid_jwks = R"({"keys": [
            {
                "kty": "RSA",
                "us
            }]

            )";

        WHEN("the jwks is parsed")
        {
            THEN("a std::exception is thrown")
            {
                REQUIRE_THROWS_AS(jwt_auth_impl::parse_RS256Keys(invalid_jwks), std::exception);
            }
        }
    }

    GIVEN("a jwks with invalid missing fields in an RSA key")
    {
        const std::string jwks_with_missing_e = R"({"keys": [
            {
                "kty": "RSA",
                "use": "sig",
                "n": "abc",                
                "kid": "key_0"
            }]
        })";
        WHEN("the jwks is parsed")
        {
            const std::vector<jwt_auth_impl::RS256Key> keys = jwt_auth_impl::parse_RS256Keys(jwks_with_missing_e);
            THEN("the missing key is not available")
            {
                REQUIRE(keys.empty());
            }
        }
    }
}

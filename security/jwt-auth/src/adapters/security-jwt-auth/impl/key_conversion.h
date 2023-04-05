#pragma once

#include <map>
#include <string>
#include <vector>

namespace jwt_auth_impl
{
    // returns a map mapping key ids to public RSA keys in PEM given by a JWKS json string.
    // may throw std::exception on any error. See the docu of the functions below for details.
    std::map<std::string, std::string> extractPubKeysPEM(const std::string& jwks_json);

    struct RS256Key
    {
        std::string id;
        std::string modulus_base64;
        std::string exponent_base64;
    };

    /*
     * see https://www.rfc-editor.org/rfc/rfc7517 for the specification of the JSON
     * extracts all RSA signature keys ( ("use" == "sig" || "key_ops" == "sign") && ()"kty" == "RSA"))
     * from a JWKS json string. Throws a std::exception if parsing fails, other kinds of keys are simply ignored.
     */
    std::vector<RS256Key> parse_RS256Keys(const std::string& jwks_json);
        
    /* 
     * converts a public RSA key to the PEM-based file format based on base64 encoded modulus and exponent.
     * Output example:
     * -----BEGIN RSA PUBLIC KEY-----
     * MIIBigKCAYEAq3DnhgYgLVJknvDA3clATozPtjI7yauqD4/ZuqgZn4KzzzkQ4BzJ
     * ar4jRygpzbghlFn0Luk1mdVKzPUgYj0VkbRlHyYfcahbgOHixOOnXkKXrtZW7yWG
     * jXPqy/ZJ/+...
     * -----END RSA PUBLIC KEY-----
     * Throws an std::exception if conversion doesn't succeed for any reasons.
     */
    std::string toPEM_PubKey(const RS256Key& key);
}

#include <catch2/catch_test_macros.hpp>
#include <adapters/security-jwt-auth/rest-jwks-rsa-key-provider.h>
#include <chrono>
#include <future>
#include <memory>
#include <thread>

SCENARIO( "JWKS RSA key provider", "[security][jwt auth]" )
{
  struct TestJWKSProvider
  {
    std::string operator()()
    {
      ++(*call_count);
      return jwks;
    }

    const std::string jwks = R"({"keys": [{"kty": "RSA","use": "sig","n": "abc","e": "def","kid": "key_0" }]})";
    std::shared_ptr<int> call_count = std::make_shared<int>(0); // share across copies.
  };
  
  GIVEN("a rest jwks RSA key provider with custom jwks provider")
  {
    TestJWKSProvider jwksProvider;
    RestJwksRSAKeyProvider restJwksProvider(jwksProvider, true);

    THEN("the provider has been called during creation")
    {
      REQUIRE(*jwksProvider.call_count == 1);
    }

    WHEN("an existing key is requested")
    {
      auto pem = restJwksProvider("key_0");
      THEN("some key in PEM format is returned")
      {
        REQUIRE(pem.find("-----BEGIN PUBLIC KEY-----") != std::string::npos);
        REQUIRE(pem.find("-----END PUBLIC KEY-----") != std::string::npos);
      }

      AND_THEN("the cache has been used instead of the provider")
      {
        REQUIRE(*jwksProvider.call_count == 1);
      }
    }
    WHEN("a non existing key is requested")
    {
      auto pem = restJwksProvider("key_29");
      THEN("an empty string is returned")
      {
        REQUIRE(pem.empty());
      }
      AND_THEN("the provider has been called once more")
      {
        REQUIRE(*jwksProvider.call_count == 2);
      }
    }
  }
  GIVEN("a custom jwks provider with invalid jwks")
  {
    TestJWKSProvider jwksProviderWithSyntaxError { R"({"keys": [{"kt": y": "RSA","use": "sig","n": "abc","e": "def","kid": "key_0" }]})", std::make_shared<int>(0) };
    
    WHEN("a rest jwks RSA key provider is created that throws")
    {
      THEN("a std exception is thrown")
      {
        REQUIRE_THROWS_AS(RestJwksRSAKeyProvider(jwksProviderWithSyntaxError, true), std::exception);
      }
    }
    WHEN("a rest jwks RSA key provider is created that does not throw")
    {
      THEN("a std exception is thrown")
      {
        REQUIRE_NOTHROW(RestJwksRSAKeyProvider(jwksProviderWithSyntaxError, false));
      }
    }
  }
  GIVEN("a rest jwks RSA key provider")
  {
    TestJWKSProvider jwksProvider;
    RestJwksRSAKeyProvider restJwksProvider(jwksProvider, true);
    WHEN("one thread keeps requesting keys that don't exist and two threads request existing threads")
    {
      std::atomic<int> keyFoundCount = 0;
      auto readExistingKey = [&]() 
      {
        for(int i = 0; i < 10; ++i)
        {
          auto pem = restJwksProvider("key_0");
          if((pem.find("-----BEGIN PUBLIC KEY-----") != std::string::npos) && (pem.find("-----END PUBLIC KEY-----") != std::string::npos))
          {
            keyFoundCount++;
          }
        }
      };

      std::atomic<int> keyNotFoundCount = 0;
      auto readNonExistingKey = [&]()
      {
        for(int i = 0; i < 10; ++i)
        {
          auto pem = restJwksProvider(std::to_string(i));          
          if(pem.empty())
          {
            keyNotFoundCount++;
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
      };

      auto reader1 = std::async(readExistingKey);
      auto reader2 = std::async(readExistingKey);
      auto cacheWriter = std::async(readNonExistingKey);

      reader1.get();
      reader2.get();
      cacheWriter.get();
      THEN("request needs to be issued 10 times")
      {
        REQUIRE(*jwksProvider.call_count == 11);
      
        AND_THEN("the existing key needs to be found 20 times")
        {
          REQUIRE(keyFoundCount == 20);

          AND_THEN("the non existing key needs to be not found 10 times")
          {
            REQUIRE(keyNotFoundCount == 10);
          }
        }
      }
    }
  }
}
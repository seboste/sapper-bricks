#include <catch2/catch_test_macros.hpp>
#include <core/core.h>
#include <ports/repo.h>
#include <ports/notifier.h>
#include <memory>

class MockRepo : public EntityRepo
{
public:    
    Entity entity;
    std::string id;    
    mutable int store_call_count = 0;
    mutable int retrieve_call_count = 0;

    MockRepo() = default;

    void Store(const Entity& entity_) override { ++store_call_count; entity = entity_; }
    std::optional<Entity> Retrieve(const std::string& id_) const override 
    { 
        ++retrieve_call_count; 
        if(id != id_) { return std::nullopt; }
        return entity; 
    }
};

class MockNotifier : public EntityNotifier
{
public:
    Entity entity;
    int changed_call_count = 0; 
    
    MockNotifier() = default;
    
    void EntityChanged(const Entity& entity_) override { ++changed_call_count; entity = entity_; }
};

SCENARIO("Core", "[core]")
{
    GIVEN("the core with mock repo")
    {
        MockRepo repo;
        repo.id = "id";
        repo.entity = Entity { "id", "string", 42 };
        std::unique_ptr<Api> api = std::make_unique<Core>(&repo, nullptr);
        
        WHEN("GetEntity is called on an existing id")
        {
            std::optional<Entity> result = api->GetEntity("id");
            THEN("Repo retrieve has been called once")
            {
                REQUIRE(repo.retrieve_call_count == 1);
            }
            AND_THEN("api returned the correct entity")
            {
                REQUIRE(result.has_value());
                REQUIRE(result.value().id == "id");
                REQUIRE(result.value().string_property == "string");
                REQUIRE(result.value().int_property == 42);
            }
        }

        WHEN("GetEntity is called on a NON existing id")
        {
            std::optional<Entity> result = api->GetEntity("invalid");
            THEN("Repo retrieve has been called once")
            {
                REQUIRE(repo.retrieve_call_count == 1);
            }
            AND_THEN("api returned nullopt")
            {
                REQUIRE(result == std::nullopt);
            }
        }

        WHEN("SetEntity is called")
        {
            Entity e { "my_id", "my_prop", 123 };
            api->SetEntity(e);
            THEN("Repo store has been called once")
            {
                REQUIRE(repo.store_call_count == 1);
            }
            AND_THEN("Repo Store is called with the correct entity")
            {
                REQUIRE(repo.entity.id == "my_id");
                REQUIRE(repo.entity.string_property == "my_prop");
                REQUIRE(repo.entity.int_property == 123);                
            }
        }
    }

    GIVEN("the core without a repo")
    {        
        std::unique_ptr<Api> api = std::make_unique<Core>(nullptr, nullptr);
        WHEN("GetEntity is called")
        {
            THEN("a runtime_error is thrown")
            {
                REQUIRE_THROWS_AS(api->GetEntity("id"), std::runtime_error);
            }
        }
        WHEN("SetEntity is called")
        {
            THEN("a runtime_error is thrown")
            {
                REQUIRE_THROWS_AS(api->SetEntity(Entity()), std::runtime_error);
            }
        }
    }

    GIVEN("the core with mock repo and mock notifier")
    {
        MockRepo repo;
        repo.id = "id";
        repo.entity = Entity { "id", "string", 42 };
        MockNotifier notifier;
        std::unique_ptr<Api> api = std::make_unique<Core>(&repo, &notifier);

         WHEN("SetEntity is called")
        {
            Entity e { "my_id", "my_prop", 123 };
            api->SetEntity(e);
            THEN("Notifier has been called once")
            {
                REQUIRE(notifier.changed_call_count == 1);
            }
            AND_THEN("Notifier is called with the correct entity")
            {
                REQUIRE(notifier.entity.id == "my_id");
                REQUIRE(notifier.entity.string_property == "my_prop");
                REQUIRE(notifier.entity.int_property == 123);                
            }
        }
    }
}

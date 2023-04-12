#include <catch2/catch_test_macros.hpp>
#include <adapters/repo-postgres/impl/postgres-connection-manager.h>
#include <microservice-essentials/cross-cutting-concerns/error-forwarding-request-hook.h>
#include <pqxx/pqxx>
#include <atomic>
#include <future>
#include <thread>


SCENARIO( "Postgres Connection Manager", "[repository][postgres]" )
{
    GIVEN("a test postgres connection manager")
    {
        int create_counter = 0;
        PostgresConnectionManager cmngr([&](){ ++create_counter; return nullptr; }, 3, std::chrono::milliseconds(1));

        THEN("the connection factory has been called once")
        {
            REQUIRE(create_counter == 1);
        }

        WHEN("some operation is executed on the connection")
        {
            int operation_called_counter = 0;
            cmngr.WithConnection([&](pqxx::connection* c) { ++operation_called_counter; }, "some operation");

            THEN("the connection factory has been called once more (because connection factory returns always nullptr in this test)")
            {
                REQUIRE(create_counter == 2);
            }
            AND_THEN("the operation has been called once")
            {
                REQUIRE(operation_called_counter == 1);
            }
        }

        WHEN("an operation throws a pqxx::broken_connection")
        {
            int operation_called_counter = 0;

            REQUIRE_THROWS_AS(
                cmngr.WithConnection([&](pqxx::connection* c) { ++operation_called_counter; throw pqxx::broken_connection(); }, "exception op"),
                mse::ErrorForwardingException
                );
            
            THEN("the operation has been called 4 times")
            {
                REQUIRE(operation_called_counter == 4);
            }
        }

        WHEN("two threads simultaniously execute operations on the connection")
        {
            std::atomic<bool> operation_active = false;
            auto background_operation = std::async([&]()
            {
                for(int i=0; i<50; ++i)
                {
                    cmngr.WithConnection([&](pqxx::connection* c){
                        if(operation_active)
                        {
                            //should have exclusive access
                            throw std::runtime_error("simultaneous access");
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(4));
                    }, "op1");
                }
            });

            for(int i=0; i<50; ++i)
            {
                cmngr.WithConnection([&](pqxx::connection* c){                    
                    operation_active = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    operation_active = false;
                }, "op2");
            }
            REQUIRE_NOTHROW(background_operation.get());
        }
    }    
}


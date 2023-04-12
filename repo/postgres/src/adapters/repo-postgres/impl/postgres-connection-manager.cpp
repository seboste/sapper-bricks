#include "postgres-connection-manager.h"

#include <microservice-essentials/cross-cutting-concerns/error-forwarding-request-hook.h>
#include <microservice-essentials/cross-cutting-concerns/exception-handling-request-hook.h>
#include <microservice-essentials/reliability/retry-request-hook.h>
#include <microservice-essentials/request/request-processor.h>

PostgresConnectionManager::PostgresConnectionManager(const ConnectionFactory& connection_factory, uint32_t max_retry_count, std::chrono::system_clock::duration first_retry_interval)
    : _connection_factory(connection_factory)
    , _max_retry_count(max_retry_count)
    , _first_retry_interval(first_retry_interval)
{
    _connection = createConnection();
}

std::unique_ptr<pqxx::connection> PostgresConnectionManager::createConnection() const
{
    if(!_connection_factory)
    {
        throw std::runtime_error("invalid connection factory");
    }
    
    return _connection_factory();
}

void PostgresConnectionManager::WithConnection(const std::function<void(pqxx::connection*)>& op, const std::string& op_name)
{
    using namespace std::chrono_literals;
        
    mse::RequestIssuer(std::string("postgres:") + op_name, mse::Context())        
        .With(mse::ErrorForwardingRequestHook::Parameters().IncludeAllErrorCodes()) //forward all error codes to handler via exception
        .With(mse::RetryRequestHook::Parameters(std::make_shared<mse::BackoffGaussianJitterDecorator>(
            std::make_shared<mse::ExponentialRetryBackoff>(_max_retry_count, _first_retry_interval), 
            _first_retry_interval / 10.0)))
        .With(mse::ExceptionHandlingRequestHook::Parameters(    //map pqxx::broken_connection to unavailable => retry with new connection.
            { std::make_shared<mse::ExceptionHandling::ExceptionOfTypeMapper<pqxx::broken_connection>>(
            mse::ExceptionHandling::Definition{ mse::Status { mse::StatusCode::unavailable, "" }, mse::LogLevel::invalid, true })}))
        .Process([&](mse::Context&)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            try
            {            
                if(!_connection)
                {
                    _connection = createConnection();
                }
                op(_connection.get());
            }
            catch(const pqxx::broken_connection&)
            {
                //reset the connection if broken...
                _connection = nullptr;
                throw; //...and communicate this to the outside
            }
            return mse::Status::OK;
        });
}

void PostgresConnectionManager::WithConnection(const ConnectionOp& op, const std::string& op_name)
{
    WithConnection([&](pqxx::connection* c){
        if(c == nullptr)
        {
            throw std::runtime_error("invalid connection");
        }
        op(*c);
    }, op_name);
}

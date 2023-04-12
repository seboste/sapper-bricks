#pragma once

#include <pqxx/pqxx>
#include <chrono>
#include <functional>
#include <mutex>
#include <memory>

class PostgresConnectionManager
{
public:
    typedef std::function<std::unique_ptr<pqxx::connection>()> ConnectionFactory;
    typedef std::function<void(pqxx::connection&)> ConnectionOp;

    PostgresConnectionManager(const ConnectionFactory& connection_factory, uint32_t max_retry_count, std::chrono::system_clock::duration first_retry_interval);
    virtual ~PostgresConnectionManager() = default;
    
    void WithConnection(const ConnectionOp& op, const std::string& op_name);
public:
    void WithConnection(const std::function<void(pqxx::connection*)>& op, const std::string& op_name); //allow nullptr connections for testing
    
protected:
    std::unique_ptr<pqxx::connection> createConnection() const;

private:
    ConnectionFactory _connection_factory;
    std::unique_ptr<pqxx::connection> _connection;
    std::mutex _mutex;
    uint32_t _max_retry_count;
    std::chrono::system_clock::duration _first_retry_interval;
};


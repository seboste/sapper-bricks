#include "mse-context-carrier.h"

MSEContextCarrier::MSEContextCarrier(mse::Context& context)
    : _context(context)
{
}

opentelemetry::nostd::string_view MSEContextCarrier::Get(opentelemetry::nostd::string_view key) const noexcept
{
    return _context.AtOr(std::string(key), "");        
}

void MSEContextCarrier::Set(opentelemetry::nostd::string_view key, opentelemetry::nostd::string_view value) noexcept
{
    _context.GetMetadata().erase(std::string(key));
    _context.GetMetadata().insert({std::string(key), std::string(value)});
}

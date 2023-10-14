#pragma once

#include <microservice-essentials/context.h>
#include <opentelemetry/context/propagation/text_map_propagator.h>

/**
 * Carrier class that allows importing/exporting of tracing headers from and to a microservice-essentials context
*/
class MSEContextCarrier : public opentelemetry::context::propagation::TextMapCarrier
{
public:
    MSEContextCarrier(mse::Context& context);
    virtual ~MSEContextCarrier() = default;

    virtual opentelemetry::nostd::string_view Get(opentelemetry::nostd::string_view key) const noexcept override;
    virtual void Set(opentelemetry::nostd::string_view key, opentelemetry::nostd::string_view value) noexcept override;

private:
    mse::Context& _context;
};

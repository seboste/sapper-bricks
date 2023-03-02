#pragma once

#include <ports/model.h>
#include <string>
#include <optional>

class Api
{
public:
    virtual ~Api() = default;
    
    virtual void SetEntity(const Entity& entity) = 0;
    virtual std::optional<Entity> GetEntity(const std::string& id) const = 0;
};

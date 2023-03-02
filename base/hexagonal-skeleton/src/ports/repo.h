#pragma once

#include <ports/model.h>
#include <string>
#include <optional>

class EntityRepo
{
public:    
    virtual ~EntityRepo() = default;
    
    virtual void Store(const Entity& entity) = 0;
    virtual std::optional<Entity> Retrieve(const std::string& id) const = 0;
};

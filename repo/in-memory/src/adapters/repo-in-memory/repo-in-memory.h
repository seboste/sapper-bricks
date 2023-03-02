#pragma once

#include <ports/repo.h>
#include <map>

class InMemoryRepo : public EntityRepo
{
public:
    virtual ~InMemoryRepo() = default;
    
    virtual void Store(const Entity& entity) override;
    virtual std::optional<Entity> Retrieve(const std::string& id) const override;
private:
    std::map<std::string, Entity> _data;
};

#include "repo-in-memory.h"

void InMemoryRepo::Store(const Entity& entity)
{
    _data[entity.id] = entity;
}

std::optional<Entity> InMemoryRepo::Retrieve(const std::string& id) const
{
    const auto cit = _data.find(id);
    if(cit == _data.end())
    {
        return std::nullopt;
    }
    return cit->second;
}

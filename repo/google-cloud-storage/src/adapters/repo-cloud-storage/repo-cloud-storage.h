#pragma once

#include <ports/repo.h>

class CloudStorageRepo : public EntityRepo
{
public:
    CloudStorageRepo(const std::string& bucket);
    virtual ~CloudStorageRepo() = default;
    
    virtual void Store(const Entity& entity) override;
    virtual std::optional<Entity> Retrieve(const std::string& id) const override;
private:
    std::string _bucket;
};

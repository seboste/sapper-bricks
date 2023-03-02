#pragma once

#include <ports/api.h>
#include <ports/notifier.h>
#include <ports/repo.h>

class Core : public Api
{
public:
    Core(EntityRepo* repo, EntityNotifier* notifier);

    virtual void SetEntity(const Entity& entity) override;
    virtual std::optional<Entity> GetEntity(const std::string& id) const override;

private:
    EntityRepo* _repo = nullptr;
    EntityNotifier* _notifier = nullptr;
};

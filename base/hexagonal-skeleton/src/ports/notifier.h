#pragma once
#include <ports/model.h>

class EntityNotifier
{
public:    
    virtual ~EntityNotifier() = default;
    
    virtual void EntityChanged(const Entity& entity) = 0;
};

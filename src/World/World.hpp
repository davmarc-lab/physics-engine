#pragma once

#include "../Entity/Entity.hpp"

#include <algorithm>
#include <vector>

class World {
protected:
    vector<Entity *> objects;

public:
    World() {}

    virtual void addEntity(Entity *entity) = 0;

    inline void removeEntity(Entity *entity) {
        if (!entity)
            return;
        auto itr = find(this->objects.begin(), this->objects.end(), entity);
        if (itr == this->objects.end())
            return;
        this->objects.erase(itr);
    }

    virtual void step(const float dt) = 0;

    ~World() = default;
};

#pragma once

#include "../Transform/Transform.hpp"

class Collider {
protected:
    Transform* trasform;

public:

    virtual void createCollider(const vector<vec3> coords) = 0;
    
    virtual bool testCollision(Transform* transform,
        Collider* collider,
        Transform* colliderTransform) = 0;
};


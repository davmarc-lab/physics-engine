#pragma once

#include "../../Entity/Collider/Collision.hpp"

class Solver {
public:
    virtual void solve(vector<Collision> collisions, const float dt) = 0;
};

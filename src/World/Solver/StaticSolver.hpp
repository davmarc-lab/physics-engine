#pragma once

#include "Solver.hpp"
#include "../PhysicWorld.hpp"

class StaticSolver : public Solver {
public:

    inline virtual void solve(vector<Collision> collisions, const float dt) override {
        // solve collisions with static objects
        for (Collision collision: collisions) {
            Entity* first = collision.getEntities().first;
            Entity* second = collision.getEntities().second;

            if (!first->isStatic() && second->isStatic()) {
                vec3 force = first->getMass() * -PhysicWorld::gravity;

                first->addForce(force);
                first->addVelocity(force / first->getMass() * first->getMotionTime());
                first->setMotionTime(0);
                continue;
            }

            if (!second->isStatic() && first->isStatic()) {
                vec3 force = second->getMass() * -PhysicWorld::gravity;

                if (second->getForce() != vec3(0)) {
                    second->addForce(force);
                    second->addVelocity(-PhysicWorld::gravity * second->getMotionTime());
                    // get normal or direction and calculate the point on the sphere
                    // second->setPosition(second->getPosition() * second->getScale() + vec3(0, 1, 0) * glm::abs(first->getPosition() * first->getScale() - second->getPosition() * second->getScale()));
                }

                second->setMotionTime(0);
                continue;
            }
        }
    }
};

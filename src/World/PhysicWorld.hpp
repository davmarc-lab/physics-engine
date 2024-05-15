#pragma once

#include "../Entity/Collider/Collider.hpp"
#include "../Entity/Collider/Collision.hpp"
#include "Solver/Solver.hpp"
#include "World.hpp"
#include <GLFW/glfw3.h>

class PhysicWorld : public World {
private:
    vector<Solver*> solvers;
    vector<Collision> collisions;

public:
    static inline vec3 gravity = vec3(0, -9.81f, 0);
    static inline float t0 = -1;
    static inline float tc = 0;

    PhysicWorld() {}

    inline void initEngine() {
        PhysicWorld::t0 = glfwGetTime();
    }

    inline static void jump(Entity *entity, const float dt) {
        // jump
        // entity->setPosition(entity->getPosition() + vec3(0, 1.f, 0));
    }

    inline void addSolver(Solver *solver) { this->solvers.push_back(solver); }

    inline void removeSolver(Solver *solver) {
        if (!solver)
            return;
        auto itr = find(this->solvers.begin(), this->solvers.end(), solver);
        if (itr == this->solvers.end())
            return;
        this->solvers.erase(itr);
    }

    inline virtual void addEntity(Entity *entity) override {
        entity->setForce(entity->getMass() * PhysicWorld::gravity);
        this->objects.push_back(entity);
    }

    inline void step(const float dt) override {
        this->tc = glfwGetTime();

        for (const auto elem : this->objects) {

            // F = m * a
            if (!elem->isStatic()) {
                elem->setVelocity(elem->getForce() / elem->getMass() * elem->getMotionTime());
            }
        }

        this->resolveCollision(dt);

        for (const auto elem : this->objects) {
            elem->addMotionTime(dt);
            elem->setPosition(elem->getPosition() + elem->getVelocity() * dt);
        }
    }

    inline void resolveCollision(const float dt) {

        for (const auto a : this->objects) {
            for (const auto b : this->objects) {
                a->setCollider(a->getCollider());
                b->setCollider(b->getCollider());

                if (a == b)
                    break;
                if (!a->getCollider() || !b->getCollider())
                    continue;

                if (a->isStatic() && b->isStatic())
                    continue;

                // get colllisions point
                bool res = a->getCollider()->testCollision(a->getTransform(), b->getCollider(), b->getTransform());
                // if collision points are colliding then append to collisions
                if (res) {
                    this->collisions.push_back(Collision(a, b));
                }
            }
        }

        // solve all collisions
        for (const auto sol : this->solvers) {
            sol->solve(this->collisions, this->tc - this->t0);
        }
        this->collisions.clear();
    }

    static inline float getTime() { return PhysicWorld::tc - PhysicWorld::t0; }

    ~PhysicWorld() = default;
};

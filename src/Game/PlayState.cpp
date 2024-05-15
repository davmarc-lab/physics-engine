#include "PlayState.hpp"

#include "../Camera/Camera.hpp"
#include "../Color/Color.hpp"
#include "../Entity/Cube.hpp"
#include "../Entity/Cubemap.hpp"
#include "../Entity/Sphere.hpp"
#include "../Entity/Plane.hpp"
#include "../Entity/Collider/SphereCollider.hpp"
#include "../World/PhysicWorld.hpp"
#include "../World/Solver/StaticSolver.hpp"

#include "../Menu/IGEntity.hpp"
#include "../Menu/IGMenu.hpp"

PlayState PlayState::playState;

CubeEntity *cube, *tmp;
Sphere *sphere;
PlaneEntity *plane;
Cubemap *skybox;
Shader cubeShader, planeShader, skyboxShader;

IGMenu *entityMenu;

PhysicWorld *pWorld = new PhysicWorld();

void PlayState::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cubeShader = Shader("./resources/shaders/vertexShader.glsl", "./resources/shaders/fragmentShader.glsl");
    cubeShader.use();
    cubeShader.setMat4("projection", projection);

    Texture texture = Texture("./resources/textures/web-dirt.png");
    texture.createTexture();

    cube = new CubeEntity();
    cube->createVertexArray();
    cube->setScale(vec3(1));
    cube->applyTransformation(vec3(1.2, 8, -2), vec3(1), vec3(0), 0);
    cube->attachTexture(texture);
    cube->setCollider(new CubeCollider(cube->getTransform()));
    cube->setDynamic();

    tmp = new CubeEntity();
    tmp->createVertexArray();
    tmp->applyTransformation(vec3(0, 4, -2), vec3(1), vec3(0), 0);
    tmp->attachTexture(texture);
    tmp->setCollider(new CubeCollider(tmp->getTransform()));

    sphere = new Sphere();
    sphere->createVertexArray();
    sphere->applyTransformation(vec3(0, 2, -4), vec3(1), vec3(0), 0);
    sphere->setCollider(new SphereCollider(sphere->getTransform()));
    sphere->setDynamic();

    planeShader = Shader("./resources/shaders/vertexShader.glsl", "./resources/shaders/fragmentShader.glsl");
    planeShader.use();
    planeShader.setMat4("projection", projection);
    planeShader.setInt("color", 1);

    plane = new PlaneEntity(color::RED);
    plane->createVertexArray();

    skyboxShader = Shader("./resources/shaders/vertexShaderSkybox.glsl", "./resources/shaders/fragmentShaderSkybox.glsl");
    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setInt("skybox", 0);

    skybox = new Cubemap();
    skybox->createVertexArray();

    pWorld->addEntity(cube);
    // pWorld->addEntity(tmp);
    pWorld->addEntity(plane);

    pWorld->addSolver(new StaticSolver());

    // imgui
    entityMenu = new IGEntity(cube);

    camera.moveCamera(vec3(0, 0, 3));

    pWorld->initEngine();
}

void PlayState::clean() {}

void PlayState::pause() {}

void PlayState::resume() {}

GLenum val = GL_FALSE;

void PlayState::handleEvent(GameEngine *engine) {
    auto window = engine->getWindow()->getWindow();

    const float dt = engine->getDeltaTime();

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    y = HEIGHT - y;
    vec2 pos = vec2(x, y);

    float cameraVelocity = camera.getCameraVelocity() * engine->getDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // lockKey(GLFW_KEY_W);
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cube->addAcceleration(vec3(-20 * dt, 0, 0));
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        // cube jump
        // PhysicWorld::jump(cube, dt);
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
}

void updatePosition(Entity *ent) {
    // reset model matrix
    ent->setModelMatrix(ent->refreshModelMatrix());
}

void PlayState::update(GameEngine *engine) {
    cubeShader.use();
    cubeShader.setMat4("view", camera.getViewMatrix());

    planeShader.use();
    planeShader.setMat4("view", camera.getViewMatrix());

    skyboxShader.use();
    skyboxShader.setMat4("view", mat4(mat3(camera.getViewMatrix())));

    pWorld->step(engine->getDeltaTime());

    updatePosition(sphere);
    updatePosition(cube);
}

void PlayState::draw(GameEngine *engine) {
    skybox->draw(skyboxShader);
    plane->draw(planeShader);
    // sphere->draw(cubeShader);
    cube->draw(cubeShader);
    // tmp->draw(cubeShader);
    entityMenu->render();
}

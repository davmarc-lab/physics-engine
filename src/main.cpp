#include "Application/Application.hpp"
#include "Lib.hpp"

#define GAME_NAME "3D Project"

int main() {
    Application var = Application(WIDTH, HEIGHT, GAME_NAME);
    var.launch();

    /* Window window = Window(GAME_NAME, WIDTH, HEIGHT);

    // time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // enable depth test for 3D
    window.enableGL(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Start of game loop

    Game game = Game(WIDTH, HEIGHT);
    game.init(&window);

    while (!glfwWindowShouldClose(window.getWindow())) {
        // manage frames time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(1.f, 1.f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input
        window.processCloseInput();
        game.processInput(deltaTime, window);

        // update game state
        game.update(deltaTime);

        // render
        game.render();

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    game.clear();
    window.terminateWindow(); */
    return 0;
}

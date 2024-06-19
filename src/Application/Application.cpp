#include "Application.hpp"

#include "../Game/Game.hpp"
#include "../Game/IntroState.hpp"
#include "../Window/Window.hpp"

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"

void Application::launch() {
    // instanciate window
    Window window = Window(this->name.c_str(), this->width, this->height);
    GameEngine game = GameEngine(&window, this->width, this->height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(),
                                 true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    game.init();

    game.changeState(IntroState::instance());

    float lastFrame = 0;

    while (!glfwWindowShouldClose(window.getWindow()) && game.isRunning()) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();

        float currentFrame = glfwGetTime();
        game.setDeltaTime(currentFrame - lastFrame);
        lastFrame = currentFrame;

        // handle input
        window.processCloseInput();
        game.processInput();

        // update game
        game.update();

        // render game
        game.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window.getWindow());

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.terminateWindow();
    game.clean();
}

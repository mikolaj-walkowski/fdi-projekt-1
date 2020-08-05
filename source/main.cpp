#include <iostream>
#include <exception>

#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <evil_macros.hpp>
#include <simulation.hpp>
#include <ui.hpp>

// Listenery do GLFW
void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW ERROR #%d: %s\n", error, description);
}

int main(int argc, char** argv)
{
    // Inicjalizacja GLFW
    glfwSetErrorCallback(errorCallback);
    enforce(glfwInit(), std::runtime_error("Failed to initialise GLFW!"));
    onExit(glfwTerminate());

    // Tworzenie okna
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    enforce(
        window = glfwCreateWindow(1280, 720, "FDI Projekt nr 1", nullptr, nullptr), 
        std::runtime_error("Failed to create window!")
    );
    onExit(glfwDestroyWindow(window));
    glfwMakeContextCurrent(window);
    glm::ivec2 fboSize;

    // Ładowanie OpenGLa
    enforce(gladLoadGL(), std::runtime_error("Failed to load OpenGL!"));

    // Inicjalizacja ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    onExit(
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    );
 
    // Musimy załadować czcionkę do polskich znaków, domyślnie ImGui ładuje tylko ASCII
    #if _WIN32
        loadFont("C:\\Windows\\Fonts\\Arial.ttf", 16);
    #endif

    auto clearColor = glm::vec4(0,0,0.2,1);
    auto state = SimulationState::STOPPED;
    SimulationSettings settings;
    Simulation simulation;
    std::vector<glm::dvec2> results;

    bool simulationRunning = false;
    int simulationSpeed = 1;

    //---------------------------------GŁÓWNA PĘTLA--------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &fboSize.x, &fboSize.y);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        switch(state)
        {
            case SimulationState::STOPPED:
            showSimulationConfigWindow(state, settings);
            if(state == SimulationState::RUNNING) 
            {
                simulation.init(settings);
                results.clear();
            }
            break;

            case SimulationState::RUNNING:
            for(int i=0; i<simulationSpeed; ++i)
            {
                simulation.update();
                results.push_back(glm::dvec2(simulation.time(), simulation.detectorPressure()));
            }
            //break pominięty celowo żeby żeby wykorzystać ten sam kod na renderowanie do RUNNING oraz PAUSED
            
            case SimulationState::PAUSED:
            simulation.render(*(ImGui::GetBackgroundDrawList()), glm::vec2(64), glm::vec2(fboSize) - 128.0f);
            showSimulationControlWindow(state, simulationSpeed, results);
            break;
        }

        ImGui::Render();
        glViewport(0, 0, fboSize.x, fboSize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
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

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW ERROR #%d: %s\n", error, description);
}

int fboWidth, fboHeight;
void fboSizeCallback(GLFWwindow* window, int width, int height) 
{
    fboWidth = width;
    fboHeight = height;
}

int main(int, char**)
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
        window = glfwCreateWindow(1024, 768, "FDI Projekt nr 1", nullptr, nullptr), 
        std::runtime_error("Failed to create window!")
    );
    onExit(glfwDestroyWindow(window));
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &fboWidth, &fboHeight);
    glfwSetFramebufferSizeCallback(window, fboSizeCallback);

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
    SimulationSettings settings;

    bool simulationRunning = false;
    int simulationSpeed = 1;

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if(simulationRunning) 
        {
            ImGui::Begin("...", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            
            ImGui::SliderInt("Szybkość", &simulationSpeed, 1, 4, "%dx");
            ImGui::Button("Pauza");
            ImGui::SameLine();
            if(ImGui::Button("Zatrzymaj symulację")) 
                simulationRunning = false;
            ImGui::NewLine();
            ImGui::End();
        } 
        else 
        {
            if(showSimulationConfigWindow(settings)) 
                simulationRunning = true;
        }

        ImGui::Render();
        glViewport(0, 0, fboWidth, fboHeight);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
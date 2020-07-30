#include <iostream>
#include <exception>

#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <evil_macros.hpp>

const auto YELLOW = glm::vec4(1,1,0,1);

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
        window = glfwCreateWindow(1024,768, "FDI Projekt nr 1", nullptr, nullptr), 
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
    ImGuiIO& io = ImGui::GetIO(); 
    auto *fonts = io.Fonts;
    ImWchar16 unicodeGlyphRanges[] = {
        0x20, 0xFF,   //ASCII
        0x100, 0x17F, //Latin Extended-A (polskie znaki)
        0x370, 0x3FF, //Greka
        0
    };
    #if _WIN32
        fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16, NULL, unicodeGlyphRanges);
    #endif

    auto clearColor = glm::vec4(0,0,0.2,1);
    auto containerDimensions = glm::vec2(40, 40);
    int maxNumParticles = static_cast<int>(containerDimensions.x * containerDimensions.y),
        numParticles = maxNumParticles/2,
        detectorDelay = 100;
    float collisionTollerance = 0.05f,
          maxInitialVelocity = 5,
          detectorY = 20, 
          detectorHeight = 5,
          dt = 1;

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        
        ImGui::Begin("Ustawienia symulacji");
            if(ImGui::CollapsingHeader("Cząstki")) 
            {
                ImGui::TextColored(YELLOW, "Stan początkowy");
                ImGui::SliderInt("Liczba", &numParticles, 1, maxNumParticles, "n = %d");
                ImGui::SliderFloat("Max. prędkość", &maxInitialVelocity, 0, 100, "V = %.2f");

                ImGui::TextColored(YELLOW, "Zderzenia");
                ImGui::SliderFloat("Tolerancja", &collisionTollerance, 0, 0.1f, "d = %.2fR");
            }
            if(ImGui::CollapsingHeader("Zbiornik"))
            {
                ImGui::TextColored(YELLOW, "Wymiary zbiornika");
                ImGui::SliderFloat("Szerokość", &containerDimensions.x, 20, 100, "L = %.2fR");
                ImGui::SliderFloat("Wysokość", &containerDimensions.y, 20, 100, "H = %.2fR");
                maxNumParticles = static_cast<int>(containerDimensions.x * containerDimensions.y);

                ImGui::TextColored(YELLOW, "Detektor");
                ImGui::SliderFloat("Położenie", &detectorY, 0, containerDimensions.y, "h = %.2fR");
                ImGui::SliderFloat("Wysokość ", &detectorHeight, 0, 20, "l = %.2fR");
                ImGui::SliderInt("Opóźnienie", &detectorDelay, 0, 500, "T = %dδt");
            }
            ImGui::Separator();
            ImGui::SliderFloat("Krok symulacji", &dt, 0.05f, 2, "δt = %.3f");

            if(ImGui::Button("ROZPOCZNIJ SYMULACJĘ", ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
                std::cerr << "Jeszcze nie zaimplementowano.";

        ImGui::End();

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
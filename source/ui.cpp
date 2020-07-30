#include <cassert>
#include <algorithm>
#include <imgui.h>
#include <ui.hpp>
#include <evil_macros.hpp>

#define HEADING_COLOR ImVec4(1,1,0,1)

void tooltip(const char *text) {
    if(ImGui::IsItemHovered()) 
        ImGui::SetTooltip(text);
}

ImFont* loadFont(const std::string &fontPath, float fontSize) {

    static ImWchar16 unicodeGlyphRanges[] = {
        0x20, 0xFF,   //ASCII
        0x100, 0x17F, //Latin Extended-A (polskie znaki)
        0x370, 0x3FF, //Greka
        0
    };

    auto *fontAtlas = ImGui::GetIO().Fonts;
    return fontAtlas->AddFontFromFileTTF(fontPath.c_str(), fontSize, NULL, unicodeGlyphRanges);
}

void showSimulationConfigWindow(SimulationState &state, SimulationSettings &settings) 
{
    assertf(state == SimulationState::STOPPED, "Nieprawidłowy stan: %s", simulationStateToString[state]);

    const int maxNumParticles = static_cast<int>(settings.containerWidth * settings.containerHeight / 4);
    settings.numParticles = std::min(settings.numParticles, maxNumParticles);

    ImGui::Begin("Ustawienia symulacji", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if(ImGui::CollapsingHeader("Cząstki")) 
    {
        ImGui::TextColored(HEADING_COLOR, "Stan początkowy");
        ImGui::SliderInt("Liczba", &settings.numParticles, 1, maxNumParticles, "n = %d");
        ImGui::SliderFloat("Max. prędkość", &settings.maxInitialVelocity, 0, 100, "V = %.2f");

        ImGui::TextColored(HEADING_COLOR, "Zderzenia");
        ImGui::SliderFloat("Tolerancja", &settings.collisionTollerance, 0, 0.1f, "d = %.3fR");
        tooltip("Współczynnik tolerancji używany przy zderzeniach cząstek z innymi cząstkami oraz ze ścianami zbiornika.");
    }
    if(ImGui::CollapsingHeader("Zbiornik"))
    {
        ImGui::TextColored(HEADING_COLOR, "Wymiary zbiornika");
        ImGui::SliderFloat("Szerokość", &settings.containerWidth, 20, 100, "L = %.1fR");
        ImGui::SliderFloat("Wysokość", &settings.containerHeight, 100, 500, "H = %.1fR");

        ImGui::TextColored(HEADING_COLOR, "Detektor");
        ImGui::SliderFloat("Położenie", &settings.detectorY, 0, settings.containerHeight, "h = %.2fR");
        tooltip("Odległość dolnej krawędzi detektora od dolnej krawędzi zbiornika.");
        ImGui::SliderFloat("Wysokość ", &settings.detectorHeight, 0, 20, "l = %.2fR");
        ImGui::SliderInt("Opóźnienie", &settings.detectorDelay, 0, 500, "T = %dδt");
        tooltip("Czas od rozpoczęcia symulacji, po którym detektor rozpocznie pomiar ciśnienia.");
    }
    ImGui::Separator();
    ImGui::SliderFloat("Krok symulacji", &settings.dt, 0.05f, 2, "δt = %.3f");
    if(ImGui::Button("ROZPOCZNIJ SYMULACJĘ", ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
        state = SimulationState::RUNNING;
    ImGui::End();
}

void showSimulationControlWindow(SimulationState &state, int &speedMultiplier)
{
    ImGui::Begin("Przebieg symulacji", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SliderInt("Szybkość", &speedMultiplier, 1, 4, "%dx");
    tooltip("Zwiększa szybkość przebiegu symulacji. Nie ma to wpływu na krok czasowy oraz dokładność symulacji.");

    switch(state) 
    {
        case SimulationState::RUNNING:
        if(ImGui::Button("Zatrzymaj")) 
            state = SimulationState::PAUSED;
        tooltip("Tymczasowo wstrzymuje przebieg symulacji.");
        break;

        case SimulationState::PAUSED:
        if(ImGui::Button("Wznów"))
            state = SimulationState::RUNNING;
        break;

        default: assertf(false, "Nieprawidłowy stan: %s", simulationStateToString[state]);
    }
    ImGui::SameLine();
    if(ImGui::Button("Zakończ symulację")) 
        state = SimulationState::STOPPED;
    ImGui::NewLine();
    ImGui::End();
}
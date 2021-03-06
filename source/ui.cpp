#include <cassert>
#include <cstring>
#include <algorithm>
#include <vector>
#include <imgui.h>
#include <ui.hpp>
#include <evil_macros.hpp>

#define NOMINMAX
#include <windows.h>
#include <direct.h>

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

    float k = 1/(settings.deltaT * settings.maxInitialVelocity);
    k = std::max(1.0f, std::min(k, 100.0f));
    ImGui::SliderFloat("Współczynnik kroku", &k, 1, 100, "κ = %.2f");
    settings.deltaT = 1/(k * settings.maxInitialVelocity);
    ImGui::Text("Krok symulacji: δt = %g", settings.deltaT);

    if(ImGui::Button("ROZPOCZNIJ SYMULACJĘ", ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
        state = SimulationState::RUNNING;
    ImGui::End();
}

void SimulationControlWindow::show(SimulationState &state, const std::vector<SimulationResult> &results)
{
    ImGui::Begin("Przebieg symulacji", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SliderFloat("Szybkość", &_targetTPS, 20, 300, "%.1fTPS");
    tooltip("Szybkość wyświetlania symulacji w krokach czasowych na sekundę.");

    auto buttonSize = ImVec2(ImGui::GetWindowContentRegionWidth()/2-8, 0);
    switch(state) 
    {
        case SimulationState::RUNNING:
        if(ImGui::Button("Zatrzymaj", buttonSize)) 
            state = SimulationState::PAUSED;
        tooltip("Tymczasowo wstrzymuje przebieg symulacji.");
        break;

        case SimulationState::PAUSED:
        if(ImGui::Button("Wznów", buttonSize))
            state = SimulationState::RUNNING;
        break;

        default: assertf(false, "Nieprawidłowy stan: %s", simulationStateToString[state]);
    }
    ImGui::SameLine();
    if(ImGui::Button("Zakończ symulację", buttonSize)) 
        state = SimulationState::STOPPED;

    if(ImGui::Button("Resetuj kamerę", buttonSize)) 
        resetCamera();
    ImGui::SameLine();

    bool doExportResults = ImGui::Button("Eksportuj wyniki", buttonSize);
    tooltip("Eksportuje wyniki do pliku .CSV");
    if(doExportResults) exportResults();

    if(!results.empty())
    {
        ImGui::TextColored(HEADING_COLOR, "Wyniki");
        ImGui::Text("p = %g", results.back().averagePressure);
    }
    ImGui::TextColored(HEADING_COLOR, "Energia Całkowita");            //Debug Fizyki
    ImGui::Text("p = %g", TotalEnergy1);                               //Debug Fizyki
    ImGui::End();
}

float SimulationControlWindow::targetTPS() const
{
    return _targetTPS;
}

std::string toLower(std::string str)
{
    for(auto &c : str) c = tolower(c);
    return str;
}

void ensurePathHasExtension(std::string &path, const std::string &extension)
{
    int i = static_cast<int>(path.size()) - 1;
    for(; i>=0; --i)
    {
        if(path[i] == '\\' || path[i] == '/') //Nie znaleziono rozszerzenia
        {
            path += extension;
            return;
        }
        if(path[i] == '.')
        {
            if(toLower(path.substr(i)) != toLower(extension)) //Jeśli rozszerzenie się nie zgadza, dodaj nowe rozszerzenie na końcu
                path += extension;
            return;
        }
    }
    // Brak kropki/slasha/backslasha -> ścieżka względna do cwd bez rozszerzenia
    path += extension;
    return;
}

const std::string extensions[] = {
    "",
    ".csv",
    ".txt"
};

void showSimulationResultExportWindow(std::function<void(const std::string&)> doExport)
{
    static char cpath[MAX_PATH];
    static char cwd[MAX_PATH];
    static OPENFILENAME ofn;
    static bool initialised = false;

    ZeroMemory(cpath, sizeof(cpath));
    
    if(!initialised)
    {
        ZeroMemory(cwd, sizeof(cpath));
        _getcwd(cwd, sizeof(cwd));
        strcat(cwd, "\\");

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "Plik CSV\0*.csv\0Plik tekstowy\0*.txt\0\0";
        ofn.lpstrInitialDir = cwd;
        ofn.lpstrFile = cpath;
        ofn.nMaxFile = sizeof(cpath);
        ofn.lpstrTitle = "Zapisz wyniki symulacji";
        ofn.Flags = OFN_DONTADDTORECENT;

        initialised = true;
    }
    if(GetSaveFileNameA(&ofn))
    {
        std::string path(cpath);
        ensurePathHasExtension(path, extensions[ofn.nFilterIndex]);
        doExport(std::string(path));
    }
    else assertf(CommDlgExtendedError() == 0, "Dialog box error");
}
#ifndef UI_HPP
#define UI_HPP

#include <imgui.h>
#include <string>
#include <simulation.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <functional>

/** Ładuje czcionkę do użycia przez ImGui i zwraca na nią wskaźnik.
 *  @param fontPath Plik zawierający czcionkę.
 */
ImFont* loadFont(const std::string &fontPath, float fontSize);

/** Wyświetla okno do konfigurowania ustawień symulacji.
 *  @param state stan w jakim znajduje się znaleźć symulacja
 *  @param settings struktura do której zostaną wpisane ustawienia
 * */
void showSimulationConfigWindow(SimulationState &state, SimulationSettings &settings);

class SimulationControlWindow
{
    public:

    void show(SimulationState &state, const std::vector<SimulationResult> &results);

    float targetTPS() const;

    std::function<void()> resetCamera;
    std::function<void()> exportResults;

    private:
    float _targetTPS = 60;
};

void showSimulationResultExportWindow(std::function<void(const std::string&)> doExport);

#endif
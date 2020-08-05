#ifndef UI_HPP
#define UI_HPP

#include <imgui.h>
#include <string>
#include <simulation.hpp>
#include <vector>
#include <glm/glm.hpp>

/** Ładuje czcionkę do użycia przez ImGui i zwraca na nią wskaźnik.
 *  @param fontPath Plik zawierający czcionkę.
 */
ImFont* loadFont(const std::string &fontPath, float fontSize);

/** Wyświetla okno do konfigurowania ustawień symulacji.
 *  @param state stan w jakim znajduje się znaleźć symulacja
 *  @param settings struktura do której zostaną wpisane ustawienia
 * */
void showSimulationConfigWindow(SimulationState &state, SimulationSettings &settings);

/** Wyświetla okno do sterowania symulacją w trakcie jej trwania.
 *  @param state stan w jakim znajduje się znaleźć symulacja
 *  @param targetTPS szybkość z jaką powinna być wyświetlana symulacja w krokach czasowych na sekundę
 * */
void showSimulationControlWindow(SimulationState &state, float &targetTPS, const std::vector<glm::dvec2> &results);

#endif
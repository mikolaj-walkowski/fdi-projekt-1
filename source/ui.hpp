#ifndef UI_HPP
#define UI_HPP

#include <imgui.h>
#include <string>
#include <simulation.hpp>

/** Ładuje czcionkę do użycia przez ImGui i zwraca na nią wskaźnik.
 *  @param fontPath Plik zawierający czcionkę.
 */
ImFont* loadFont(const std::string &fontPath, float fontSize);

/** Wyświetla okno do konfigurowania ustawień symulacji.
 *  @return true jeśli użytkownik nacisnął przycisk "Rozpocznij symulację"
 * */
bool showSimulationConfigWindow(SimulationSettings &settings);

#endif
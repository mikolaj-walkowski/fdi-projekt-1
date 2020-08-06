#ifndef EXPORT_HPP
#define EXPORT_HPP

#include <string>
#include <vector>
#include <simulation.hpp>

/** Zapisuje wyniki symulacji do pliku .CSV
 *  @param path Ścieżka do pliku .CSV
 *  @param results Wyniki symulacji
 */
void saveSimulationResultsToCsv(const std::string &path, const std::vector<SimulationResult> &results);

#endif
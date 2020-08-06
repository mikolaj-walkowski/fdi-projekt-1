#include "export.hpp"
#include <fstream>

void saveSimulationResultsToCsv(const std::string &path, const std::vector<SimulationResult> &results)
{
    std::ofstream out(path);
    out << "Czas,\"Średnie ciśnienie\";" << std::endl;
    for(const auto &result : results)
        out << result.time << "," << result.averagePressure << ";" << std::endl;
}
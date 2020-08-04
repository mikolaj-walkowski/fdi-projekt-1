#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>
#include <imgui.h>

enum SimulationState 
{
    STOPPED, // Symulacja jeszcze nie została rozpoczęta
    RUNNING, // Symulacja trwa
    PAUSED   // Symulacja została tymczasowo zatrzymana z możliwością wznowienia
};

const char * const simulationStateToString[] = {
    "STOPPED",
    "RUNNING",
    "PAUSED"
};

struct SimulationSettings 
{
    float containerWidth = 40,
          containerHeight = 250,
          collisionTollerance = 0.05f,
          maxInitialVelocity = 5,
          /// Odległość między podłogą pudełka, a dolną krawędzią detektora
          detectorY = 20, 
          detectorHeight = 5,
          /// Krok czasowy
          deltaT = 1;

    int numParticles = 100,
        /// Ile kroków czasowych po rozpoczęciu symulacji detektor ma zacząć mierzyć ciśnienie
        detectorDelay = 100;
};

class Atom {//(1)najpierw kolizje atom
public:
    glm::dvec2 x, v;
    double R = 1;

    void tryCollide(Atom &b, double tollerance);
    void update(double deltaT);
};

class Border {//(3)nakoncu border
public:
    double l=0,h=0;
    void tryCollide(Atom &a, double tollerance);
};

class Detector {// (2)póniej z line
public:
    double l, h, boxl, momentumAbsorbed = 0;
    void tryCollide(Atom &a, double tollerance);
};

/** Obecny stan symulacji: cząstki, detektor, ustawienia początkowe, etc. */
class Simulation 
{
public:
    /** Inicjalizuje symulację przy użyciu zadanych ustawień początkowych. Generuje atomy. */
    void init(const SimulationSettings &settings);
    /** Aktualizuje stan symulacji do czasu t = time() + dt */
    void update();
    double time() const;
    double detectorPressure() const;
    void render(ImDrawList &drawList) const;

private:
    SimulationSettings settings;
    Border border;
    Detector detector;
    std::vector<Atom> atoms;
    size_t ticksPassed = 0;
};

#endif

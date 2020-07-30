#ifndef SIMULATION_HPP
#define SIMULATION_HPP

struct SimulationSettings {
    float containerWidth = 40,
          containerHeight = 250,
          collisionTollerance = 0.05f,
          maxInitialVelocity = 5,
          /// Odległość między podłogą pudełka, a dolną krawędzią detektora
          detectorY = 20, 
          detectorHeight = 5,
          /// Krok czasowy
          dt = 1;

    int numParticles = 100,
        /// Ile kroków czasowych po rozpoczęciu symulacji detektor ma zacząć mierzyć ciśnienie
        detectorDelay = 100;
};

#endif
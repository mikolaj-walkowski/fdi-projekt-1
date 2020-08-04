#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <math.h>
#include <cstdlib>
#include <time.h>
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

struct SimulationSettings {
    float containerWidth = 40,
          containerHeight = 250,
          collisionTollerance = 0.05f,
          maxInitialVelocity = 5,
          /// Odległość między podłogą pudełka, a dolną krawędzią detektora
          detectorY = 20, 
          detectorHeight = 5,
          /// Krok czasowy
          delta_t = 1;

    int numParticles = 100,
        /// Ile kroków czasowych po rozpoczęciu symulacji detektor ma zacząć mierzyć ciśnienie
        detectorDelay = 100;
};

srand(time(0));
double R=1, V;
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
class atom;
class border {//(3)nakoncu border
public:
    double h,d,l;
    bool set() {
        SimulationSettings a;
        h = a.containerHeight, l = a.containerWidth,d=a.collisionTollerance;
    }
    void isColliding(atom* a) {
        if ((a->x.x) + d >= this->l || (a->x.x) - d <= 0) {
            a->v.x = -(a->v.x);
        }

        if ((a->x.y) + d >= this->h || (a->x.y) - d <= 0) {
            a->v.y = -(a->v.y);
        }
    }
};
class atom {//(1)najpierw kolizje atom
public:
    glm::dvec2 x, v;
    float R = 1, d,delta_t;
    void set() {//cząsteczki mogą wpaść na siebie
        SimulationSettings *a=new SimulationSettings;
        d = a->collisionTollerance;
        delta_t = a->delta_t;
        x = glm::dvec2(fRand(0, a->containerWidth), fRand(0, a->containerHeight));
        v = glm::dvec2(fRand(-a->maxInitialVelocity), a->maxInitialVelocity)), fRand(-a->maxInitialVelocity), a->maxInitialVelocity));
        delete a;
    }
    void colision(atom* b) {
        if ((x - b->x).length <= 2 * R + d) {
            auto n = glm::normalize(x - b->x);
            auto c1 = n * glm::dot(n, v);
            auto c2 = n * glm::dot(n, b->v);
            v = v - c1 + c2;
            b->v = b->v - c2 + c1;
        }
    }
    void update_positon() {
        x += (v * delta_t);
        v.y -= delta_t;
        x.x -= pow(delta_t, 2) / 2;
    }
};
class line {// (2)póniej z line
public:
    double h, l, res = 0,boxl;
    int time, collisions, M;
    void set(double h1, double l1, double M1) {
        SimulationSettings a;
            h = a.detectorHeight,
            l=a.detectorY,
            time = 1,
            collisions = 0,
            M = a.detectorDelay;
            boxl = a.containerWidth;
    }
    double isColliding(atom* a) {
        if (time % M != 0) {
            if ((a->x.y) + d >= h && (a->x.y) + d <= h + l && (a->x.x) + d >= boxl) {
                ++time;
                res += 2 * a->v.x;
                return -1.f;
            }
        }
        else {
            time = 1;
            double res2 = res / (l * M);
            res = 0;
            return res2;
        }
    }
} barometer;

#endif

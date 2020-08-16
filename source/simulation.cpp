#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include "simulation.hpp"
#include <algorithm>

double R=1, V;
double ToltalEnergy1 ;//do debugu
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void Border::tryCollide(Atom &a, double tollerance) 
{
    if ((a.x.x) + a.R + tollerance >= this->l)
        a.v.x = -abs(a.v.x);

    if((a.x.x) - a.R - tollerance <= 0)
        a.v.x = abs(a.v.x);

    if ((a.x.y) + a.R + tollerance >= this->h) 
        a.v.y = -abs(a.v.y);

    if ((a.x.y) - a.R - tollerance <= 0) {
        a.v.y = abs(a.v.y);
        a.isCollidingBottom = 1;
    }
}

void Atom::tryCollide(Atom &b, double tollerance) 
{
    if (
        glm::dot(v - b.v, b.x - x) > 0 && //Sprawdź czy ten atom leci w stronę drugiego atomu
        //co jeżeli atom który sprawdzamy odsuwa się wolniej od atomu który go uderza 
        glm::distance(x, b.x) <= R + b.R + tollerance
    ) {
        auto n = glm::normalize(x - b.x);
        auto c1 = n * glm::dot(n, v);
        auto c2 = n * glm::dot(n, b.v);
        v = v - c1 + c2;
        b.v = b.v - c2 + c1;
    }
}

void Atom::update(double deltaT)
{
    ToltalEnergy1 += x.x + (pow(v.length,2)/2.f);           //debug fizyki
    if(!isCollidingBottom)
    {
    x += v * deltaT;
    v.y -= deltaT;
    x.x -= pow(deltaT, 2) / 2;
    }
    else {
        isCollidingBottom = 0;
        x += v * deltaT;
    }
    //wydaje mi się że problemem jest to że atom przyśpiesza nawet jak jest w stanie spoczynku na dolnej krawędzi zbiornika to powinno to naprawić
}

void Detector::tryCollide(Atom &a, double tollerance) {
    if (
        (a.x.y) + a.R + tollerance >= h && 
        (a.x.y) - a.R - tollerance <= h + l && 
        (a.x.x) + a.R + tollerance >= boxl &&
        a.v.x >= 0
    ) {
        momentumAbsorbed += 2 * a.v.x;
    }
}

glm::dvec2 dvec2Rand(double maxLength) 
{
    double arg = fRand(0, 2*M_PI);
    return fRand(0,maxLength) * glm::dvec2(cos(arg), sin(arg));
}

void Simulation::init(const SimulationSettings &settings) 
{
    this->settings = settings;

    border = Border();
    border.l = settings.containerWidth;
    border.h = settings.containerHeight;

    detector = Detector();
    detector.h = settings.detectorY;
    detector.l = settings.detectorHeight;
    detector.boxl = settings.containerWidth;

    atoms.clear();
    for(size_t i=0; i<settings.numParticles; ++i)
        atoms.push_back(Atom{
            glm::dvec2(fRand(0, settings.containerWidth), fRand(0, settings.containerHeight)),
            dvec2Rand(settings.maxInitialVelocity)
        });
}

void Simulation::update() 
{
    ++ticksPassed;
    ToltalEnergy1 = 0;                          //debug fizyki
    for(auto &atom : atoms) 
    {
        atom.update(settings.deltaT);
        if(ticksPassed > settings.detectorDelay)
            detector.tryCollide(atom, settings.collisionTollerance);
        border.tryCollide(atom, settings.collisionTollerance);
    }

    for(size_t i=0; i<atoms.size()-1; ++i)
        for(size_t j=i+1; j<atoms.size(); ++j)
            atoms[i].tryCollide(atoms[j], settings.collisionTollerance);
}

double Simulation::time() const 
{
    return ticksPassed * settings.deltaT;
}

double Simulation::detectorPressure() const 
{
    if(ticksPassed <= settings.detectorDelay) return 0;
    size_t measurementTime = (ticksPassed - settings.detectorDelay) * settings.deltaT;
    return detector.momentumAbsorbed / measurementTime / detector.l;
}

/// Signum
float sgn(float x)
{
    if(x > 0) return  1;
    if(x < 0) return -1;
    return 0;
}

// Funkcje na macierze transformacji w 2D
glm::mat3 translate2D(glm::vec2 t) {
    glm::mat3 result(1);
    result[2] = glm::vec3(t,1);
    return result;
}

glm::mat3 scale2D(glm::vec2 factors)
{
    glm::mat3 result(1);
    result[0][0] = factors[0];
    result[1][1] = factors[1];
    return result;
}

/// Horyzontalny min: zwraca najmniejszy element z danego wektora
float hmin(glm::vec2 v)
{
    return std::min(v.x, v.y);
}

const ImU32 BORDER_COLOR = ImGui::ColorConvertFloat4ToU32(ImVec4(1,0,0,1)),
            PARTICLE_COLOR = ImGui::ColorConvertFloat4ToU32(ImVec4(1,1,0,1)),
            DETECTOR_COLOR = ImGui::ColorConvertFloat4ToU32(ImVec4(0,1,0,1));

void Simulation::render(ImDrawList &drawList, glm::vec2 position, glm::vec2 dimensions) const 
{
    // Skalujemy oraz przesuwamy zbiornik, detektor, atomy 
    // tak by zmieściły się w zadanym obszarze przy zachowaniu proporcji
    auto borderDimensions = glm::vec2(border.l, border.h);
    float scale = hmin(dimensions / borderDimensions); 

    auto borderDrawnDimensions = borderDimensions * scale;
    position += (0.5f * (dimensions - borderDrawnDimensions));

    glm::mat3 transform = 
        translate2D(0.5f * borderDrawnDimensions + position) * 
        scale2D(glm::vec2(scale, -scale)) * 
        translate2D(-0.5f * borderDimensions);

    for(const auto &atom : atoms) 
    {
        auto drawnPosition = glm::vec2(transform * glm::vec3(atom.x,1));
        drawList.AddCircle(drawnPosition, atom.R * scale, PARTICLE_COLOR);
    }

    drawList.AddRect(glm::vec2(transform * glm::vec3(0,0,1)), glm::vec2(transform * glm::vec3(borderDimensions,1)), BORDER_COLOR);
    drawList.AddLine(
        glm::vec2(transform * glm::vec3(detector.boxl,detector.h, 1)), 
        glm::vec2(transform * glm::vec3(detector.boxl,detector.h+detector.l,1)), 
        DETECTOR_COLOR
    );
}
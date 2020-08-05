#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include "simulation.hpp"

double R=1, V;
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void Border::tryCollide(Atom &a, double tollerance) 
{
    if ((a.x.x) + tollerance >= this->l || (a.x.x) - tollerance <= 0)
        a.v.x = -(a.v.x);

    if ((a.x.y) + tollerance >= this->h || (a.x.y) - tollerance <= 0)
        a.v.y = -(a.v.y);
}

void Atom::tryCollide(Atom &b, double tollerance) 
{
    if (glm::distance(x, b.x) <= R + b.R + tollerance) 
    {
        auto n = glm::normalize(x - b.x);
        auto c1 = n * glm::dot(n, v);
        auto c2 = n * glm::dot(n, b.v);
        v = v - c1 + c2;
        b.v = b.v - c2 + c1;
    }
}

void Atom::update(double deltaT) 
{
    x += v * deltaT;
    v.y -= deltaT;
    x.x -= pow(deltaT, 2) / 2;
}

void Detector::tryCollide(Atom &a, double tollerance) {
    if ((a.x.y) + tollerance >= h && (a.x.y) + tollerance <= h + l && (a.x.x) + tollerance >= boxl)
        momentumAbsorbed += abs(2 * a.v.x);
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
    for(auto &atom : atoms) 
    {
        atom.update(settings.deltaT);
        border.tryCollide(atom, settings.collisionTollerance);
        if(ticksPassed > settings.detectorDelay)
            detector.tryCollide(atom, settings.collisionTollerance);
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

void Simulation::render(ImDrawList &drawList) const 
{
    for(const auto &atom : atoms) 
        drawList.AddCircle(atom.x, atom.R, ImGui::ColorConvertFloat4ToU32(ImVec4(1,1,0,1)));

    drawList.AddRect(ImVec2(0,0), ImVec2(border.l, border.h), ImGui::ColorConvertFloat4ToU32(ImVec4(1,0,0,1)));
    drawList.AddLine(ImVec2(detector.boxl,detector.h), ImVec2(detector.boxl,detector.h+detector.l), ImGui::ColorConvertFloat4ToU32(ImVec4(0,1,0,1)));
}
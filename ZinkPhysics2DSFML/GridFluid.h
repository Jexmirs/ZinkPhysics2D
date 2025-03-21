#ifndef GRIDFLUID_H
#define GRIDFLUID_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Vector2D.h"

class FluidSimulation {
private:
    int N;
    float dt, viscosity;
    std::vector<float> u, v, u_prev, v_prev, p, divergence, density, density_prev;

public:
    FluidSimulation(int gridSize, float timeStep, float vis);

    void addSource(std::vector<float>& x, std::vector<float>& s, float dt);
    void setBoundary(int b, std::vector<float>& x);
    void diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt);
    void linearSolve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c);
    void update(float timestep);
    void render(sf::RenderWindow& window);
};

#endif

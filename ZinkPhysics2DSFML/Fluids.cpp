#ifndef FLUID_SIMULATION_H
#define FLUID_SIMULATION_H

#include "FluidGrid.h"
#include <SFML/Graphics.hpp>

class FluidSimulation {
public:
    FluidGrid fluidGrid;
    float timestep;

    FluidSimulation(int width, int height, float cellSize, float timestep)
        : fluidGrid(width, height, cellSize), timestep(timestep) {
        fluidGrid.addDensity(50, 50, 100.0f);
        fluidGrid.addVelocity(50, 50, Vector2D(1.0f, 0.0f));
    }

    void update() {
        fluidGrid.step(timestep);
    }

    void render(sf::RenderWindow& window) {
        fluidGrid.render(window);
    }
};

#endif

#include <SFML/Graphics.hpp>
#include "FluidSimulation.h"
#include "PhysicsSimulation.h"

enum class VisualizationType {
    FluidSimulation,
    PhysicsSimulation
};

int main() {
    const int width = 64;
    const int height = 64;
    const float cellSize = 10.0f;
    const float timestep = 0.01f;

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(width * cellSize),
        static_cast<unsigned int>(height * cellSize)),
        "Fluid Simulation");

    FluidSimulation fluidSim(width, timestep, 0.0001f);
    PhysicsSimulation physicsSim;

    VisualizationType currentVisualization = VisualizationType::FluidSimulation;

    fluidSim.setFluidAmount(1.5f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F) {
                    currentVisualization = VisualizationType::FluidSimulation;
                }
                if (event.key.code == sf::Keyboard::P) {
                    currentVisualization = VisualizationType::PhysicsSimulation;
                }
            }
        }

        window.clear(sf::Color::White);

        if (currentVisualization == VisualizationType::FluidSimulation) {
            fluidSim.update(timestep);
            fluidSim.render(window);
        }
        else if (currentVisualization == VisualizationType::PhysicsSimulation) {
            physicsSim.update(window);
        }

        window.display();
    }

    return 0;
}

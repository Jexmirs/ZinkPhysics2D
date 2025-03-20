#include <SFML/Graphics.hpp>
#include "FluidSimulation.h"
#include "PhysicsSimulation.h"

enum class VisualizationType {
    FluidSimulation,
    PhysicsSimulation
};

int main() {
    const int width = 100;
    const int height = 100;
    const float cellSize = 5.0f;
    const float timestep = 0.1f;

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(width * cellSize),
        static_cast<unsigned int>(height * cellSize)),
        "Visualization");

    FluidSimulation fluidSim(width, height, cellSize);
    PhysicsSimulation physicsSim;

    fluidSim.addParticle(Vector2D(50, 50), Vector2D(1.0f, 0.0f), 1.0f);
    fluidSim.addParticle(Vector2D(60, 50), Vector2D(0.0f, 1.0f), 1.0f);

    VisualizationType currentVisualization = VisualizationType::FluidSimulation;

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

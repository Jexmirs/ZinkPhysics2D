#include <SFML/Graphics.hpp>
#include "FluidGrid.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "2D Fluid Simulation");
    FluidGrid fluid(100, 100, 6.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        fluid.step(0.1f);
        window.clear();
        fluid.render(window);
        window.display();
    }

    return 0;
}

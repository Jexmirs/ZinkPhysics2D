#ifndef CHARTS_H
#define CHARTS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Chart {
public:
    Chart()
        : xPos(0), yPos(0), chartWidth(200), chartHeight(100), chartColor(sf::Color::Black), chartTitle("") {
    }

    Chart(float x, float y, float width, float height, const std::string& title, sf::Color color = sf::Color::Black)
        : xPos(x), yPos(y), chartWidth(width), chartHeight(height), chartColor(color), chartTitle(title) {
    }

    void addData(float value) {
        data.push_back(value);
        if (data.size() > static_cast<size_t>(chartWidth)) {
            data.erase(data.begin());
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::VertexArray lines(sf::LinesStrip);
        for (size_t i = 0; i < data.size(); ++i) {
            float x = xPos + i;
            float y = yPos + chartHeight - data[i];
            lines.append(sf::Vertex(sf::Vector2f(x, y), chartColor));
        }

        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text titleText(chartTitle, font, 14);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(xPos, yPos - 20);
        window.draw(titleText);
        window.draw(lines);
    }

private:
    float xPos, yPos, chartWidth, chartHeight;
    sf::Color chartColor;
    std::vector<float> data;
    std::string chartTitle;
};
#endif

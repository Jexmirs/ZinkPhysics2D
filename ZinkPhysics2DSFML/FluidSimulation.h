#ifndef FLUIDSIMULATION_H
#define FLUIDSIMULATION_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <algorithm>

class FluidSimulation {
private:
    int N;
    float dt, viscosity;
    std::vector<float> u, v, u_prev, v_prev;
    std::vector<float> density, density_prev;

public:
    FluidSimulation(int gridSize, float timeStep, float vis)
        : N(gridSize), dt(timeStep), viscosity(vis),
        u(N* N, 0), v(N* N, 0), u_prev(N* N, 0), v_prev(N* N, 0),
        density(N* N, 0), density_prev(N* N, 0) {
    }

    void addSource(std::vector<float>& x, const std::vector<float>& s, float dt) {
        for (size_t i = 0; i < x.size(); i++) {
            x[i] += dt * s[i];
        }
    }

    void setBoundary(int b, std::vector<float>& x) {
        for (int i = 1; i < N - 1; i++) {
            x[i] = b == 1 ? -x[i + N] : x[i + N];
            x[i + (N - 1) * N] = b == 1 ? -x[i + (N - 2) * N] : x[i + (N - 2) * N];
        }

        for (int i = 1; i < N - 1; i++) {
            x[i * N] = b == 2 ? -x[(i + 1) * N] : x[(i + 1) * N];
            x[i * N + (N - 1)] = b == 2 ? -x[(i + 1) * N + (N - 2)] : x[(i + 1) * N + (N - 2)];
        }
    }

    void applyGravity(std::vector<float>& v, float gravity, float dt) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                v[i + j * N] += gravity * dt;
            }
        }
    }

    void advect(std::vector<float>& d, std::vector<float>& d0, std::vector<float>& u, std::vector<float>& v, float dt) {
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                float x = i - dt * u[i + j * N];
                float y = j - dt * v[i + j * N];

                x = std::max(0.5f, std::min(x, float(N - 1.5f)));
                y = std::max(0.5f, std::min(y, float(N - 1.5f)));

                int i0 = int(x);
                int j0 = int(y);
                int i1 = i0 + 1;
                int j1 = j0 + 1;

                float s1 = x - i0;
                float s0 = 1.0f - s1;
                float t1 = y - j0;
                float t0 = 1.0f - t1;

                d[i + j * N] = s0 * (t0 * d0[i0 + j0 * N] + t1 * d0[i0 + j1 * N]) +
                    s1 * (t0 * d0[i1 + j0 * N] + t1 * d0[i1 + j1 * N]);
            }
        }
    }

    void update(float timestep) {
        density_prev = density;

        applyGravity(v, 9.8f, timestep);

        advect(density, density_prev, u, v, timestep);
    }

    void render(sf::RenderWindow& window) {
        sf::VertexArray pixels(sf::Points, N * N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                float d = density[i + j * N] * 255;
                sf::Color color(0, 0, d);

                pixels[i + j * N].position = sf::Vector2f(i * 10, j * 10);
                pixels[i + j * N].color = color;
            }
        }
        window.draw(pixels);
    }

    void addFluidSource(int xStart, int yStart, float amount) {
        for (int i = xStart - 5; i < xStart + 5; i++) {
            for (int j = yStart - 5; j < yStart + 5; j++) {
                if (i >= 0 && i < N && j >= 0 && j < N) {
                    density[i + j * N] = amount;
                }
            }
        }
    }

    void setFluidAmount(float liters) {
        // Convert liters to grid cells
        int totalCells = N * N;
        int fluidCells = int(liters * 1000);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (fluidCells > 0) {
                    density[i + j * N] = 1.0f;
                    fluidCells--;
                }
                else {
                    density[i + j * N] = 0.0f;
                }
            }
        }

        setBoundary(2, density);
    }
};

#endif

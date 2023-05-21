#include <iostream>
#include "polygon.h"
#include <SFML/Graphics.hpp>

void plot_points(sf::RenderWindow &wind, std::vector<vertex>& vertices)
{
    for (const auto v : vertices)
    {
        sf::RectangleShape s{sf::Vector2f(5, 5)};
        double xs = (10.0 - 0.0) / 620.0;
        double ys = (0.0 - 10.0) / 520.0;
        double xp = (v.x - 0.0) / xs;
        double yp = (v.y - 10.0) / ys;
        s.setPosition(static_cast<float>(xp), static_cast<float>(yp));
        wind.draw(s);
    }
}

void plot_edges(sf::RenderWindow &wind, std::deque<edge> &edges)
{
    std::vector<std::array<sf::Vertex, 2>> lines;
    for (const auto &e : edges)
    {
        double xp1 = (e.start->x - 0.0) / ((10.0 - 0.0) / 620.0);
        double yp1 = (e.start->y - 10.0) / ((0.0 - 10.0) / 520.0);
        double xp2 = (e.next->start->x - 0.0) / ((10.0 - 0.0) / 620.0);
        double yp2 = (e.next->start->y - 10.0) / ((0.0 - 10.0) / 520.0);
        const std::array<sf::Vertex, 2> line{{
            sf::Vertex(sf::Vector2f(
                static_cast<float>(xp1 + 2.),
                static_cast<float>(yp1 + 2.))),
            sf::Vertex(sf::Vector2f(
                static_cast<float>(xp2 + 2.),
                static_cast<float>(yp2 + 2.))),
        }};
        wind.draw(line.data(), 2, sf::Lines);
    }
}

int main()
{
    std::vector<vertex> vertices1{{0.0, 0.0}, {1.0, -1.0}, {2.0, 1.0}, {3.0, 0.0}, {4.0, 3.0}, {2.0, 4.0}, {1.0, 3.0}, {0.72, 1.58}, {0.24, 1.84}};

    std::vector<vertex> vertices2{{2, 2}, {3, 1}, {3.98, 1.56}, {5, 0}, {4.56, 3.02}, {6, 2}, {6.95, 4.59}, {5.34, 4.21},
                                  {5, 7}, {4.16, 6}, {3.38, 7}, {2.68, 5.68}, {3.1, 4.82}, {2.84, 4}, {2.22, 4.44}};

    polygon poly(vertices2);
    poly.makeMonotone();

    std::cout << poly;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Monotone pieces of polygon");
    window.setFramerateLimit(1);
    plot_points(window, poly.getVertices());
    plot_edges(window, poly.getEdges());

    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}

// g++ -IC:\SFML-2.5.1\include -c runme.cpp -o runme.o
// g++ -LC:\SFML-2.5.1\lib .\runme.o -o a.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows
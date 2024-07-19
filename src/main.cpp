#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <time.h>

int main()
{
    constexpr int width = 800;
    constexpr int height = 600;
    constexpr int dot_size = 5;

    std::array<sf::Color, 50> colors;
    std::srand(time(nullptr));
    for (size_t i = 0; i < 50; ++i) {
        colors[i] = sf::Color(std::rand() % 256,
                              std::rand() % 256,
                              std::rand() % 256);
    }
    
    std::vector<sf::CircleShape> circles;
    std::vector<sf::Vector2i> dot_pos;

    sf::RenderWindow window(sf::VideoMode(width, height), "voronoi",
                            sf::Style::Titlebar | sf::Style::Close);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse_pos = sf::Vector2i(event.mouseButton.x,
                                                      event.mouseButton.y);
                dot_pos.push_back(mouse_pos);

                sf::CircleShape circle(dot_size);
                circle.setFillColor(sf::Color::Red);
                circle.setPosition(mouse_pos.x - dot_size, mouse_pos.y - dot_size);
                circles.push_back(circle);
            }
        }

        window.clear();
        for (size_t i = 0; i < circles.size(); ++i) {
            window.draw(circles[i]);
        }
        window.display();
    }

    return 0;
}
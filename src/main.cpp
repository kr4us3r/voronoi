#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <time.h>
#include <iostream>

int main()
{
    // set fixed window width and height
    constexpr int width = 800;
    constexpr int height = 600;
    // set fixed point size
    constexpr int point_size = 5;

    /*  create 50 random colors in advance to
        save resources during diagram's generation */
    std::array<sf::Color, 50> colors;
    std::srand(time(nullptr));
    for (size_t i = 0; i < 50; ++i) {
        colors[i] = sf::Color(std::rand() % 256,
                              std::rand() % 256,
                              std::rand() % 256);
    }
    
    // create vectors to store points and their positions
    std::vector<sf::CircleShape> circles;
    std::vector<sf::Vector2i> point_pos;

    // initialize window with a custome style to prevent resize and fullscreen
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
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // read and store mouse click position
                    sf::Vector2i mouse_pos = sf::Vector2i(event.mouseButton.x,
                                                        event.mouseButton.y);
                    for (size_t i = 0; i < point_pos.size(); ++i) {
                        if (point_pos[i] == mouse_pos) {
                            continue;
                        }
                    }
                    point_pos.push_back(mouse_pos);

                    sf::CircleShape circle(point_size);
                    circle.setFillColor(sf::Color::Red);
                    circle.setPosition(mouse_pos.x - point_size, mouse_pos.y - point_size);
                    circles.push_back(circle);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    // to do
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    // remove all points and their stored positions
                    circles.clear();
                    point_pos.clear();
                }
            }
        }

        window.clear();
        for (size_t i = 0; i < circles.size(); ++i) {
            window.draw(circles[i]);
        }
        window.display();
    }

    std::cout << point_pos.size() << '\n';

    return 0;
}
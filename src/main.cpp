#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <time.h>
#include <cmath>
#include <algorithm>


int main()
{
    // set fixed window width and height
    constexpr int width = 800;
    constexpr int height = 600;
    // set fixed point size
    constexpr int point_size = 5;

    /*  create num_colors random colors in advance to
        save resources during diagram's generation */
    constexpr size_t num_colors = 128;
    std::array<sf::Color, num_colors> colors;
    std::srand(time(nullptr));
    for (size_t i = 0; i < num_colors; ++i) {
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

    sf::Sprite background;
    sf::Texture texture;
    sf::Image img;
    img.create(width, height, sf::Color::Black);
    texture.loadFromImage(img);
    background.setTexture(texture);

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
                    size_t size = point_pos.size();

                    auto search = std::find_if(point_pos.begin(), point_pos.end(),
                        [&](const sf::Vector2i v) {
                            return v == mouse_pos;
                    });
                    if (search != std::end(point_pos))
                        continue;

                    point_pos.push_back(mouse_pos);

                    sf::CircleShape circle(point_size);
                    circle.setFillColor(sf::Color::Red);
                    circle.setPosition(mouse_pos.x - point_size, mouse_pos.y - point_size);
                    circles.push_back(circle);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    int num_cells = point_pos.size();

                    for (int y = 0; y < height; ++y) {
                        for (int x = 0; x < width; ++x) {
                            int d_min = INT32_MAX;
                            int j = 0;
                            for (size_t i = 0; i < num_cells; ++i) {
                                int d = std::pow(point_pos[i].x - x, 2) + std::pow(point_pos[i].y - y, 2);
                                if (d < d_min) {
                                    d_min = d;
                                    j = i;
                                }
                            }
                            img.setPixel(x, y, colors[j % num_colors]);
                        }
                    }
                    texture.loadFromImage(img);
                    background.setTexture(texture);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    // remove all points and their stored positions
                    circles.clear();
                    point_pos.clear();
                    // reset background
                    img.create(width, height, sf::Color::Black);
                    texture.loadFromImage(img);
                    background.setTexture(texture);
                }
            }
        }

        window.clear();
        window.draw(background);
        for (size_t i = 0; i < circles.size(); ++i) {
            window.draw(circles[i]);
        }
        window.display();
    }

    return 0;
}
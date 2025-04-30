#include <game.h>
#include <ctime>
#include <cmath>
#include <algorithm>

Game::Game() : window(sf::VideoMode({width, height}), "voronoi",
                      sf::Style::Titlebar | sf::Style::Close) {
    // seed the randomizer
    std::srand(std::time(nullptr));
    // generate num_colors random colors
    for (size_t i = 0; i < num_colors; ++i) {
        colors[i] = sf::Color(std::rand() % 256,
                              std::rand() % 256,
                              std::rand() % 256);
    }
}

void Game::setBlackBackground() {
    img = sf::Image({width, height}, sf::Color::Black);
    texture.update(img);
    background.setTexture(texture);
}

void Game::render() {
    window.draw(background);
    for (size_t i = 0; i < circles.size(); ++i) {
        window.draw(circles[i]);
    }
}

void Game::createPoint(sf::Vector2i position) {
    sf::Vector2i mouse_pos = sf::Vector2i(position.x,
                                          position.y);
    /* find if such a position is already present in the array
    to prevent multiple circles at identic positions */
    auto search = std::find_if(point_pos.begin(), point_pos.end(),
        [=](sf::Vector2i v) {
            return mouse_pos == v;
        });
    if (search != std::end(point_pos))
        return;

    point_pos.push_back(mouse_pos);

    sf::CircleShape circle(point_size);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition({float(mouse_pos.x - point_size), float(mouse_pos.y - point_size)});
    circles.push_back(circle);
}

void Game::generateVoronoi() {
    int num_cells = point_pos.size();
    int d_min;
    int j;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            d_min = INT32_MAX;
            j = 0;

            for (size_t i = 0; i < num_cells; ++i) {
                int d = std::pow(point_pos[i].x - x, 2) +
                        std::pow(point_pos[i].y - y, 2);
                if (d < d_min) {
                    d_min = d;
                    j = i;
                }
            }
            img.setPixel({(unsigned)x, (unsigned)y}, colors[j % num_colors]);
        }
    }
    texture.update(img);
    background.setTexture(texture);
}

void Game::reset() {
    // remove all points and their stored positions
    circles.clear();
    point_pos.clear();
    setBlackBackground();
}

void Game::run() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            else if (const auto* buttonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (buttonPressed->button == sf::Mouse::Button::Left) {
                    createPoint(buttonPressed->position);
                }
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    generateVoronoi();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    reset();
                }
            }
        }
        window.clear();
        render();
        window.display();
    }
}
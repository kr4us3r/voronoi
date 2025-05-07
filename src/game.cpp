#include <game.h>
#include <ctime>
#include <algorithm>
#include <limits>

Game::Game() : window(sf::VideoMode({width, height}), "voronoi",
                      sf::Style::Titlebar | sf::Style::Close),
               img({width, height}, sf::Color::Black),
               texture(img),
               background(texture) {
    // seed the randomizer
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    // generate num_colors random colors
    for (std::size_t i = 0; i < num_colors; ++i) {
        colors[i] = sf::Color(static_cast<uint8_t>(std::rand() % 256),
                              static_cast<uint8_t>(std::rand() % 256),
                              static_cast<uint8_t>(std::rand() % 256));
    }
    circle.setRadius(static_cast<float>(point_size));
    circle.setFillColor(sf::Color::Red);
    point_pos.reserve(num_colors);
}

void Game::setBlackBackground() {
    for (unsigned x = 0; x < width; ++x) {
        for (unsigned y = 0; y < height; ++y) {
            img.setPixel({x, y}, sf::Color::Black);
        }
    }
    texture.update(img);
}

void Game::render() {
    window.clear();
    window.draw(background);
    for (const auto& pos : point_pos) {
        circle.setPosition({static_cast<float>(pos.x - point_size),
                            static_cast<float>(pos.y - point_size)});
        window.draw(circle);
    }
    window.display();
}

void Game::createPoint(sf::Vector2i position) {
    // prevent duplicates
    if (std::find(point_pos.begin(), point_pos.end(), position) != point_pos.end())
        return;

    point_pos.push_back(position);
}

void Game::generateVoronoi() {
    std::size_t num_cells = point_pos.size();
    if (num_cells == 0 || num_cells > num_colors)
        return;

    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            unsigned dist_min = std::numeric_limits<unsigned>::max();
            std::size_t j = 0;

            for (std::size_t i = 0; i < num_cells; ++i) {
                int diff_x = point_pos[i].x - x;
                int diff_y = point_pos[i].y - y;
                unsigned dist = diff_x * diff_x + diff_y * diff_y;
                if (dist < dist_min) {
                    dist_min = dist;
                    j = i;
                }
            }
            img.setPixel({x, y}, colors[j]);
        }
    }
    texture.update(img);
}

void Game::reset() {
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
        render();
    }
}
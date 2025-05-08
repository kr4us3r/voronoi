#include <game.h>
#include <ctime>
#include <limits>
#include <thread>
#include <vector>

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
    point_set.reserve(num_colors);
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
    if (!point_set.insert(position).second)
        return;

    point_pos.push_back(position);
}

void Game::generateVoronoi() {
    std::size_t num_cells = point_pos.size();
    if (num_cells == 0 || num_cells > num_colors)
        return;

    unsigned num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;

    unsigned rows_per_thread = height / num_threads;
    unsigned remaining_rows = height % num_threads;

    std::vector<std::thread> threads;

    auto compute_chunk = [this, num_cells](unsigned start_y, unsigned end_y) {
        for (unsigned y = start_y; y < end_y; ++y) {
            for (unsigned x = 0; x < width; ++x) {
                unsigned dist_min = std::numeric_limits<unsigned>::max();
                std::size_t j = 0;

                for (std::size_t i = 0; i < num_cells; ++i) {
                    int diff_x = point_pos[i].x - static_cast<int>(x);
                    int diff_y = point_pos[i].y - static_cast<int>(y);
                    unsigned dist = static_cast<unsigned>(diff_x*diff_x + diff_y*diff_y);
                    if (dist < dist_min) {
                        dist_min = dist;
                        j = i;
                    }
                }
                img.setPixel({x, y}, colors[j]);
            }
        }
    };

    unsigned current_y = 0;
    for (unsigned t = 0; t < num_threads; ++t) {
        unsigned extra_row = (t < remaining_rows) ? 1 : 0;
        unsigned chunk_size = rows_per_thread + extra_row;
        unsigned start_y = current_y;
        unsigned end_y = start_y + chunk_size;

        threads.emplace_back(compute_chunk, start_y, end_y);
        current_y = end_y;
    }

    for (auto& thread : threads) {
        thread.join();
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

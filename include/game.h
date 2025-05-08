#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <array>
#include <functional>
#include <unordered_set>

class Game {
    // set fixed window width and height
    const unsigned width = 1280u;
    const unsigned height = 800u;
    // set fixed point size
    const int point_size = 4;

    // number of colors to create in advance
    static constexpr std::size_t num_colors = 512;
    std::array<sf::Color, num_colors> colors;
    // stored positions of points
    std::vector<sf::Vector2i> point_pos;

    // set of points positions to prevent dupicates
    struct Vector2iHash {
        std::size_t operator()(const sf::Vector2i& v) const {
            return (std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1));
        }
    };
    std::unordered_set<sf::Vector2i, Vector2iHash> point_set;

    sf::RenderWindow window;
    sf::Image img;
    sf::Texture texture;
    sf::Sprite background;
    // shape to be drawn on mouse click
    sf::CircleShape circle;

    void setBlackBackground();
    void render();
    void createPoint(sf::Vector2i);
    void generateVoronoi();
    void reset();

public:
    Game();
    void run();
};

#endif // GAME
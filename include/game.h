#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <array>

class Game {
    // set fixed window width and height
    const unsigned width = 1024u;
    const unsigned height = 768u;
    // set fixed point size
    const int point_size = 4;

    // number of colors to create in advance
    static constexpr std::size_t num_colors = 256;
    // vectors to store colors and points positions
    std::array<sf::Color, num_colors> colors;
    std::vector<sf::Vector2i> point_pos;

    sf::RenderWindow window;
    sf::Image img;
    sf::Texture texture;
    sf::Sprite background;
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
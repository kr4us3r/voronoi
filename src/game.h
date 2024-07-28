#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

class Game {
    // set fixed window width and height
    const int width = 800;
    const int height = 600;
    // set fixed point size
    const int point_size = 5;

    /*  number of colors to create in advance to
        save resources during diagram's generation */
    static const size_t num_colors = 128;
    std::array<sf::Color, num_colors> colors;

    // create vectors to store points and their positions
    std::vector<sf::CircleShape> circles;
    std::vector<sf::Vector2i> point_pos;

    sf::RenderWindow window;

    sf::Sprite background;
    sf::Texture texture;
    sf::Image img;

    void setBlackBackground();
    void render();
    void createPoint(sf::Event&);
    void generateVoronoi();
    void reset();

public:
    Game();
    void run();
};

#endif
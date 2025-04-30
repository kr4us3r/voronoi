#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <array>

class Game {
    // set fixed window width and height
    const unsigned width = 800;
    const unsigned height = 600;
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

    sf::Texture texture{sf::Vector2u({width, height})};
    sf::Sprite background{texture};
    sf::Image img{{width, height}, sf::Color::Black};

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
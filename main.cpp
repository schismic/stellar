#include "Starmap/Starmap.hpp"

bool adjust_color(sf::Color &color, const sf::Color &target);
void update_velocity(double &velocity);
void update_rotation(double &rotation, sf::IntRect &interior);
void update_tilt(double &tilt, sf::IntRect &interior);

int main ()
{   
    // Window
    sf::RenderWindow window {sf::VideoMode(WINDOW_X, WINDOW_Y), "Stellar", sf::Style::Titlebar | sf::Style::Close};
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    // Executable icon
    sf::Image icon;
    icon.loadFromFile("res/icon.ico");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // Spaceship sprite
    sf::Texture ship_texture;
    ship_texture.loadFromFile("res/ship.png");
    ship_texture.setSmooth(true);
    sf::Sprite ship;
    ship.setTexture(ship_texture);
    sf::IntRect interior {
                        (static_cast<int>(ship_texture.getSize().x) - WINDOW_X)/2,
                        (static_cast<int>(ship_texture.getSize().y) - WINDOW_Y)/2,
                        WINDOW_X,
                        WINDOW_Y
                        };

    size_t next_color {1}; // palette index
    sf::Color galaxy_color = PALETTE[0]; // current color
    Starmap starmap {STARMAP_SIZE, WINDOW_X, WINDOW_Y, PALETTE};
    sf::Clock clock;

    while (window.isOpen())
    {   
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
                window.close();
        }

        // Handle input
        update_velocity(velocity);
        update_rotation(rotation, interior);
        update_tilt(tilt, interior);
        
        // Calculate distance traveled
        sf::Time time = clock.restart();
        distance -= time.asSeconds()*velocity;

        // Change galaxy color relative to the distance
        if (distance <= 0) {
            distance = 2.5;
            // Step 1 RGB towards next color
            if (adjust_color(galaxy_color, PALETTE.at(next_color)))
                next_color = (next_color + 1) % PALETTE.size();
        }

        // Update the ship sprite
        ship.setTextureRect(interior);
        
        // Render and display
        window.clear(galaxy_color);
        starmap.render(window, velocity, rotation, tilt);
        window.draw(ship);
        window.display();
    }
}

bool adjust_color(sf::Color &color, const sf::Color &next_color)
    // Transition seamlessly to the next color
    {   
        if (color.r != next_color.r)
            color.r += (color.r > next_color.r) ? -1 : 1;
        if (color.b != next_color.b)
            color.b += (color.b > next_color.b) ? -1 : 1;
        if (color.g != next_color.g)
            color.g += (color.g > next_color.g) ? -1 : 1;
        return (color == next_color) ? true : false;
    }
    

void update_velocity(double &velocity)
    // Check for input and update velocity accordingly
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && velocity <= MAX_VELOCITY) {
            velocity += THRUST + ACCELERATION;
        }
        else if (velocity > MIN_VELOCITY) {
            velocity -= (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) ? THRUST : 0;
        }
        else
            velocity = MIN_VELOCITY;
        
    }

void update_rotation(double &rotation, sf::IntRect &interior)
    // Check for input and update rotation and interior movement accordingly
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && std::fabs(rotation) <= MAX_ROTATION) {
            rotation += ROTATION;
            interior.left += ROTATION;
        }  
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && std::fabs(rotation) <= MAX_ROTATION) {
            rotation -= ROTATION;
            interior.left -= ROTATION;
        }
        else {
            if (rotation > ROTATION) {
                rotation -= ROTATION;
                interior.left -= ROTATION;
            }
            else if (rotation < -ROTATION) {
                rotation += ROTATION;
                interior.left += ROTATION;
            }
            else {
                rotation = 0;
            }
        }
    }

void update_tilt(double &tilt, sf::IntRect &interior)
    // Check for input and update tilt and interior movement accordingly
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && std::fabs(tilt) <= MAX_TILT) {
            tilt += TILT;
            interior.top -= MAX_TILT;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && std::fabs(tilt) <= MAX_TILT) {
            tilt -= TILT;
            interior.top += MAX_TILT;
        }
        else {
            if (tilt > TILT) {
                tilt -= TILT;
                interior.top += MAX_TILT;
            }
            else if (tilt < -TILT) {
                tilt += TILT;
                interior.top -= MAX_TILT;
            }
            else {
                tilt = 0;
            }
        }
    }

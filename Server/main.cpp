#define CHPP_INCLUDE_ALL
#include "config.hpp"

int main()
{
    RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    CircleShape shape(100.f);
    shape.setFillColor(Color::Green);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}

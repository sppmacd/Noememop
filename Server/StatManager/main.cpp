#define CHPP_INCLUDE_ALL
#include <fstream>
#include "config.hpp"

vector<Vertex> players;
vector<Vertex> pomemeons;
vector<Vertex> clients;

int maxplayers = 0;
int maxpomemeons = 0;
int maxclients = 0;

int main()
{
    RenderWindow window(sf::VideoMode(1000, 500), "Pomemeon Stat Manager");
    window.setFramerateLimit(60);

    sf::View view(window.getDefaultView());
    view.move(-500.f, 0.f);

    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
                view.move(5.f, 0.f);
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
                view.move(-5.f, 0.f);
        }

        if(clock.getElapsedTime().asSeconds() > 1.f)
        {
            clock.restart();
            int x,y,z;
            ifstream stream;
            stream.open("stats.txt");

            if(!stream.good())
            {
                x=y=z=1;
                cerr << "Couldn't read stat file!" << endl;
            }
            else
            {
                stream >> x >> y >> z;
            }
            stream.close();

            pomemeons.push_back(Vertex(Vector2f(pomemeons.size() * 10.f, (x * 5.f) + 1.f), Color::Red));
            clients.push_back(Vertex(Vector2f(clients.size() * 10.f, (y * 5.f) + 1.f), Color::Green));
            players.push_back(Vertex(Vector2f(players.size() * 10.f, (z * 5.f) + 1.f), Color::Blue));

            view.setSize(1000.f, 500.f);
            //view.zoom(max(max(maxclients,maxplayers),maxpomemeons));
            view.move(10.f, 0.f);
        }

        window.setView(view);
        window.clear(Color(200,200,200));
        window.draw(pomemeons.data(), pomemeons.size(), LineStrip);
        window.draw(clients.data(), clients.size(), LineStrip);
        window.draw(players.data(), players.size(), LineStrip);
        window.display();
    }

    return 0;
}

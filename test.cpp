#include <SFML/Graphics.hpp>
#include "Bounds2D.hpp"
#include "SFMLCamera.hpp"

int main()
{
    Bounds2D bounds(0,100,0,100);

    sf::VertexArray polyLine(sf::LineStrip);
    for(int i = 0; i < 100; ++i)
        polyLine.append({{rand()%100,rand()%100},sf::Color::Black});

    sf::RenderWindow window(sf::VideoMode(600,600),"testCamera");
    sf::View view;
    view.setSize(bounds.size());
    view.setCenter(bounds.center());
    window.setView(view);

    Camera camera(window.getSize(), bounds);

    sf::Event event;
    while(window.isOpen())
    {
        window.clear(sf::Color::White);
        window.draw(polyLine);
        window.display();

        window.waitEvent(event);
        if(event.type == sf::Event::Closed)
            window.close();
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();
        camera.handleEvent(event);
        window.setView(camera.getView());
    }

    return 0;
}

//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include "snake.hpp"

const float BLOCK_WIDTH = 20.0f;

const sf::Vector2<float> UP (0.f, -BLOCK_WIDTH / 2);
const sf::Vector2<float> DOWN (0.f, BLOCK_WIDTH / 2);
const sf::Vector2<float> LEFT (-BLOCK_WIDTH / 2, 0.f);
const sf::Vector2<float> RIGHT (BLOCK_WIDTH / 2, 0.f);

void pause(sf::RenderWindow*);

int main()
{
    bool hasChangedDirection = false, doneMoving = true;
    sf::Vector2f toChange(0.f, 0.f);
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Snake Game!");
    window.setFramerateLimit(30);
    
    // head snake
    Snake head(true);
    head.m_Sprite.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
    
    for (int i = 0; i < 1600; i++)
    {
        Snake* snek = new Snake(false);
        snek->m_Sprite.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
    }
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                // Close window: exit
                case sf::Event::Closed :
                    window.close();
                    break;
                // handle directional keys pressed
                case sf::Event::KeyPressed :
                    
                    if (event.key.code == sf::Keyboard::Escape)
                        pause(&window);
        
                    if (event.key.code == sf::Keyboard::W)
                    {
                        if (head.m_HeadDirection != DOWN && !hasChangedDirection)
                        {
                            toChange = UP;
                            hasChangedDirection = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S)
                    {
                        if (head.m_HeadDirection != UP && !hasChangedDirection)
                        {
                            toChange = DOWN;
                            hasChangedDirection = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::A)
                    {
                        if (head.m_HeadDirection != RIGHT && !hasChangedDirection)
                        {
                            toChange = LEFT;
                            hasChangedDirection = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::D)
                    {
                        if (head.m_HeadDirection != LEFT && !hasChangedDirection)
                        {
                            toChange = RIGHT;
                            hasChangedDirection = true;
                        }
                    }
                    break;
                case sf::Event::LostFocus :
                    pause(&window);
                    break;
                default :
                    break;
            }
        }
        hasChangedDirection = false;
            
        // Clear screen
        window.clear();
        
        // update all following snake positions
        for (int i = head.m_SnakeList.size() - 1; i > 0; i--)
        {
            head.m_SnakeList[i]->m_Sprite.setPosition(head.m_SnakeList[i - 1]->m_Sprite.getPosition());
            window.draw(head.m_SnakeList[i]->m_Sprite);
        }
        
        /*
         pseudo code for updating snakes
         
         list [1, 0, 0, 0]                   |   list [0, 0, 0, 0]
                                             |
         for (i = size - 1; i > 0; i--)      |
             list[i] = list[i-1]             |
         list[0]++                           |
                                             |
         list [2, 1, 0, 0] -> after one run  |   list [1, 0, 0, 0] -> seems to add up...
         list [3, 2, 1, 0]                   |   list [2, 1, 0, 0]
         list [4, 3, 2, 1]                   |   list [3, 2, 1, 0]
         list [5, 4, 3, 2]                   |   list [4, 3, 2, 1]
         
         */
        
        // update head snake
        if (doneMoving)
            head.m_HeadDirection = toChange;
        head.m_Sprite.move(head.m_HeadDirection);
        window.draw(head.m_Sprite);
        doneMoving = !doneMoving;
        
        window.display();
    }
    
    for (auto i : head.m_SnakeList)
        delete i;

    return EXIT_SUCCESS;
}

void pause(sf::RenderWindow* window)
{
    sf::Text countdown;
    sf::Font font;
    if (!font.loadFromFile("ARCAC___.TTF"))
        exit(1);
    countdown.setFont(font);
    countdown.setCharacterSize(40);
    countdown.setString("Press any key to continue");
    countdown.setPosition(sf::Vector2f(100, 375));
    window->clear();
    window->draw(countdown);
    window->display();
    
    sf::Event event;
    while (window->isOpen())
    {
        window->waitEvent(event);
        if (event.type == sf::Event::KeyPressed)
        {
            // display countdown before starting again
            countdown.setPosition(sf::Vector2f(375, 375));
            countdown.setString("3");
            window->clear();
            window->draw(countdown);
            window->display();
            
            sf::sleep(sf::seconds(1.f));
            countdown.setString("2");
            window->clear();
            window->draw(countdown);
            window->display();
            
            sf::sleep(sf::seconds(1.f));
            countdown.setString("1");
            window->clear();
            window->draw(countdown);
            window->display();
            
            sf::sleep(sf::seconds(1.f));
            countdown.setString("GO!");
            window->clear();
            window->draw(countdown);
            window->display();
            sf::sleep(sf::seconds(1.f));
            
            return;
        }
        if (event.type == sf::Event::Closed)
        {
            window->close();
            return;
        }
    }
}

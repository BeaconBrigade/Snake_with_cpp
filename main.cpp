
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

#include <SFML/Graphics.hpp>
#include <string>
#include "snake.hpp"

const sf::Vector2<float> UP (0.f, -BLOCK_WIDTH / 2);
const sf::Vector2<float> DOWN (0.f, BLOCK_WIDTH / 2);
const sf::Vector2<float> LEFT (-BLOCK_WIDTH / 2, 0.f);
const sf::Vector2<float> RIGHT (BLOCK_WIDTH / 2, 0.f);
static sf::Font font;

void pause(sf::RenderWindow *w);
bool endOfGame(sf::RenderWindow *w, int& score);
bool gameLoop(sf::RenderWindow&);

int main()
{
    // create the main window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Snake Game!");
    window.setFramerateLimit(24); // normal is 24
    
    while (window.isOpen())
    {
        if (!gameLoop(window))
            return EXIT_SUCCESS;
    }
    
    return EXIT_SUCCESS;
}

bool gameLoop(sf::RenderWindow& window)
{
    // game setup
    bool hasChangedDirection = false, brokeBoundaries = false, doneMoving = true, started = false;
    sf::Vector2f toChange(0.f, 0.f);
    int score, frames = 0;
    
    // head snake
    Snake head(true, true);
    
    // initial follower snakes
    for (int i = 0; i < 8; i++)
        new Snake(false, true);
        
    score = head.m_SnakeList.size();

    // food
    Food *snack = new Food();
    
    // start game with a pause
    pause(&window);
    
    // start the game loop
    while (window.isOpen())
    {
        // event handling
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
        
                    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
                    {
                        if (head.m_HeadDirection != DOWN && !hasChangedDirection)
                        {
                            toChange = UP;
                            hasChangedDirection = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
                    {
                        if (head.m_HeadDirection != UP && !hasChangedDirection)
                        {
                            toChange = DOWN;
                            hasChangedDirection = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left)
                    {
                        if (head.m_HeadDirection != RIGHT && !hasChangedDirection)
                        {
                            toChange = LEFT;
                            hasChangedDirection = true;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right)
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
        
        // eating food
        if (head.m_Sprite.getPosition() == snack->m_Sprite.getPosition())
        {
            for (int i = 0; i < 3; i++)
            {
                Snake *snek = new Snake(false, true);
                snek->m_Sprite.setPosition(head.m_Sprite.getPosition()); // so snakes don't appear in the corner
                score++;
            }
            delete snack;
            snack = new Food();
            score += 3;
        }
            
        // clear screen
        window.clear();
        
        // update all following snake positions
        for (long unsigned i = head.m_SnakeList.size() - 1; i > 0; i--)
        {
            head.m_SnakeList[i]->m_Sprite.setPosition(head.m_SnakeList[i - 1]->m_Sprite.getPosition());
            window.draw(head.m_SnakeList[i]->m_Sprite);
        }
        
        window.draw(snack->m_Sprite);
        
        // update head snake
        if (doneMoving)
            head.m_HeadDirection = toChange;
        head.m_Sprite.move(head.m_HeadDirection);
        window.draw(head.m_Sprite);
        doneMoving = !doneMoving;
        
        window.display();
        
        // prevent collision on start of game
        if (hasChangedDirection)
            started = true;
        if (started)
            frames++;
        hasChangedDirection = false;
        
        // collision with boundaries
        if (head.m_Sprite.getPosition().x < 0 || head.m_Sprite.getPosition().x > 800 - (int)BLOCK_WIDTH || head.m_Sprite.getPosition().y < 0 || head.m_Sprite.getPosition().y > 800 - (int)BLOCK_WIDTH)
            brokeBoundaries = true;
        
        // collision with self
        for (long unsigned i = 1; i < head.m_SnakeList.size(); i++)
        {
            if (head.m_Sprite.getPosition() == head.m_SnakeList[i]->m_Sprite.getPosition() && frames > 8)
                brokeBoundaries = true;
        }
        
        if (brokeBoundaries)
            break;
    }
    
    window.clear();
    
    // update all following snake positions
    for (int i = head.m_SnakeList.size() - 1; i > 0; i--)
    {
        head.m_SnakeList[i]->m_Sprite.setPosition(head.m_SnakeList[i - 1]->m_Sprite.getPosition());
        head.m_SnakeList[i]->m_Sprite.setColor(sf::Color::Red);
        window.draw(head.m_SnakeList[i]->m_Sprite);
    }
    
    window.draw(snack->m_Sprite);
    head.m_Sprite.setColor(sf::Color::Red);
    window.draw(head.m_Sprite);
    
    window.display();
    sf::sleep(sf::seconds(1));
    
    // free memory, make sure not to delete m_SnakeList[0]
    // because that was stack allocated...
    for (int i = head.m_SnakeList.size() - 1; i > 0; i--)
        delete head.m_SnakeList[i];
    
    return endOfGame(&window, score);
}

void initText(sf::Text& text, int size, const char *str, sf::Vector2f pos)
{
    if (!font.loadFromFile("ARCAC___.TTF"))
        exit(1);
    text.setFont(font);
    text.setCharacterSize(size);
    text.setString(str);
    text.setPosition(pos);
}

void pause(sf::RenderWindow *window)
{
    // string constant seperated by double spaces because of inconsistent font
    sf::Text countdown, instruction;
    initText(countdown, 40, "Press  any  key  to  continue", sf::Vector2f(100, 375));
    initText(instruction, 40, "use  wasd  to  move", sf::Vector2f(205, 425));
    
    window->clear();
    window->draw(countdown);
    window->draw(instruction);
    window->display();
    
    sf::Event event;
    while (window->isOpen())
    {
        window->waitEvent(event);
        if (event.type == sf::Event::KeyPressed)
        {
            // display countdown before starting again
            countdown.setPosition(375, 375);
            
            for (int i = 3; i > 0; i--)
            {
                countdown.setString(std::to_string(i));
                window->clear();
                window->draw(countdown);
                window->display();
                sf::sleep(sf::seconds(1.f));
            }
            
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

bool endOfGame(sf::RenderWindow *window, int& score)
{
    sf::Text gameOver, scoreText, playAgain;

    // string constant seperated by double spaces because of inconsistent font
    initText(gameOver, 50, ((score < (800 / BLOCK_WIDTH) * (800 / BLOCK_WIDTH)) ? "GAME  OVER" : "VICTORY"), sf::Vector2f(290, 375));
    initText(scoreText, 40, std::string("Score " + std::to_string(score)).c_str(), sf::Vector2f(335, 420));
    initText(playAgain, 40, "Would  you  like  to  play  again    y    n", sf::Vector2f(80, 465));
    
    window->clear();
    window->draw(gameOver);
    window->draw(scoreText);
    window->draw(playAgain);
    window->display();
    
    sf::Event event;
    while (window->isOpen())
    {
        window->waitEvent(event);
        if (event.type == sf::Event::Closed)
        {
            window->close();
            return false;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Y)
                return true;
            else if (event.key.code == sf::Keyboard::N)
                return false;
            else if (event.key.code == sf::Keyboard::Escape)
                pause(window);
        }
    }
	return false;
}

//
//  snake.cpp
//  second-attempt-sfml
//
//  Created by Ryan Cullen on 2022-04-24.
//  Copyright © 2022 Ryan Cullen. All rights reserved.
//

#include "snake.hpp"
sf::Vector2f Snake::m_HeadDirection = sf::Vector2<float>(0.f, 0.f);
std::vector<Snake*> Snake::m_SnakeList;
sf::Texture Snake::m_Texture;

const float SCALE = BLOCK_WIDTH / 20.0f;

Snake::Snake(bool isLead, bool isSnake)
{
    if (isLead)
    {
        if (!Snake::m_Texture.loadFromFile("simpleRect.png"))
            exit(1);
    }
    
    Snake::m_Sprite.setTexture(Snake::m_Texture);
    Snake::m_Sprite.setScale(SCALE, SCALE);
    
    if (isSnake)
        Snake::m_SnakeList.push_back(this);
}

Food::Food()
{
    bool flagRaised = false;
    sf::Vector2f candidatePos;
    Food::m_Sprite.setTexture(Food::m_Texture);
    Food::m_Sprite.setColor(sf::Color::Green);
    
    do
    {
        candidatePos = sf::Vector2f((rand() % (40 / (int)SCALE)) * (int)BLOCK_WIDTH, (rand() % (40 / (int)SCALE)) * (int)BLOCK_WIDTH);
        
        for (Snake* i : Food::m_SnakeList)
        {
            if (i->m_Sprite.getPosition() == candidatePos)
            {
                flagRaised = true;
                break;                
            }
        }
        
    } while (flagRaised);
    
    Food::m_Sprite.setPosition(candidatePos);
}

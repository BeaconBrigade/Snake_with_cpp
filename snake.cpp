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

Snake::Snake(bool isLead)
{
    if (isLead)
    {
        if (!Snake::m_Texture.loadFromFile("simpleRect.png"))
            exit(1);
    }
    
    Snake::m_Sprite.setTexture(Snake::m_Texture);
    
    Snake::m_SnakeList.push_back(this);
}

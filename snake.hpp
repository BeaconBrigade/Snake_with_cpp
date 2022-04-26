//
//  snake.hpp
//  second-attempt-sfml
//
//  Created by Ryan Cullen on 2022-04-24.
//  Copyright © 2022 Ryan Cullen. All rights reserved.
//

#ifndef snake_hpp
#define snake_hpp

#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include <vector>

class Snake
{
public:
    static sf::Texture m_Texture;
    static std::vector<Snake*> m_SnakeList;
    static sf::Vector2<float> m_HeadDirection;
    sf::Sprite m_Sprite;
public:
    Snake(bool isLead = false);
};


#endif /* snake_hpp */
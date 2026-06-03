#pragma once

#include <SFML/Graphics.hpp>

class Ball;

class Collision
{
public:
    static bool checkAABB(
        const sf::FloatRect& a,
        const sf::FloatRect& b
    );

    static void resolveBallBlockCollision(
        Ball& ball,
        const sf::FloatRect& blockRect
    );

    static void resolveBallBallCollision(
        Ball& a,
        Ball& b
    );

    static bool isBallBelowBottom(
        const Ball& ball,
        float bottomY
    );
};
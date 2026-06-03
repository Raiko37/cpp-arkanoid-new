#include "Collision.h"
#include "Ball.h"

#include <cmath>

bool Collision::checkAABB(
    const sf::FloatRect& a,
    const sf::FloatRect& b)
{
    return a.intersects(b);
}

void Collision::resolveBallBlockCollision(
    Ball& ball,
    const sf::FloatRect& rect)
{
    sf::FloatRect ballRect =
        ball.getGlobalBounds();

    float overlapLeft =
        (ballRect.left + ballRect.width) - rect.left;

    float overlapRight =
        (rect.left + rect.width) - ballRect.left;

    float overlapTop =
        (ballRect.top + ballRect.height) - rect.top;

    float overlapBottom =
        (rect.top + rect.height) - ballRect.top;

    bool fromLeft =
        std::abs(overlapLeft) <
        std::abs(overlapRight);

    bool fromTop =
        std::abs(overlapTop) <
        std::abs(overlapBottom);

    float minX =
        fromLeft ?
        overlapLeft :
        overlapRight;

    float minY =
        fromTop ?
        overlapTop :
        overlapBottom;

    sf::Vector2f vel =
        ball.getVelocity();

    if (std::abs(minX) < std::abs(minY))
    {
        vel.x = -vel.x;
    }
    else
    {
        vel.y = -vel.y;
    }

    ball.setVelocity(vel);
}

void Collision::resolveBallBallCollision(
    Ball& a,
    Ball& b)
{
    sf::Vector2f posA =
        a.getPosition();

    sf::Vector2f posB =
        b.getPosition();

    sf::Vector2f velA =
        a.getVelocity();

    sf::Vector2f velB =
        b.getVelocity();

    sf::Vector2f delta =
    {
        posA.x - posB.x,
        posA.y - posB.y
    };

    float distance =
        std::sqrt(
            delta.x * delta.x +
            delta.y * delta.y);

    if (distance < 0.001f)
        return;

    float radiusSum =
        a.getRadius() +
        b.getRadius();

    if (distance > radiusSum)
        return;

    sf::Vector2f normal =
    {
        delta.x / distance,
        delta.y / distance
    };

    sf::Vector2f relative =
    {
        velA.x - velB.x,
        velA.y - velB.y
    };

    float velocityAlongNormal =
        relative.x * normal.x +
        relative.y * normal.y;

    if (velocityAlongNormal > 0.f)
        return;

    float e = 1.f;

    float impulse =
        -(1.f + e) *
        velocityAlongNormal /
        2.f;

    velA.x += impulse * normal.x;
    velA.y += impulse * normal.y;

    velB.x -= impulse * normal.x;
    velB.y -= impulse * normal.y;

    a.setVelocity(velA);
    b.setVelocity(velB);
}

bool Collision::isBallBelowBottom(
    const Ball& ball,
    float bottomY)
{
    return
        ball.getPosition().y >
        bottomY;
}
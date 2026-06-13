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
    sf::Vector2f pos =
        ball.getPosition();

    sf::Vector2f vel =
        ball.getVelocity();

    float radius =
        ball.getRadius();

    float overlapLeft =
        pos.x + radius - rect.left;

    float overlapRight =
        rect.left + rect.width - (pos.x - radius);

    float overlapTop =
        pos.y + radius - rect.top;

    float overlapBottom =
        rect.top + rect.height - (pos.y - radius);

    if (
        overlapLeft <= 0.f ||
        overlapRight <= 0.f ||
        overlapTop <= 0.f ||
        overlapBottom <= 0.f)
    {
        return;
    }

    float minOverlap = overlapLeft;
    int side = 0;

    if (overlapRight < minOverlap)
    {
        minOverlap = overlapRight;
        side = 1;
    }

    if (overlapTop < minOverlap)
    {
        minOverlap = overlapTop;
        side = 2;
    }

    if (overlapBottom < minOverlap)
    {
        side = 3;
    }

    switch (side)
    {
    case 0:
        pos.x = rect.left - radius - 0.1f;
        vel.x = -std::abs(vel.x);
        break;

    case 1:
        pos.x = rect.left + rect.width + radius + 0.1f;
        vel.x = std::abs(vel.x);
        break;

    case 2:
        pos.y = rect.top - radius - 0.1f;
        vel.y = -std::abs(vel.y);
        break;

    case 3:
        pos.y = rect.top + rect.height + radius + 0.1f;
        vel.y = std::abs(vel.y);
        break;
    }

    ball.setPosition(pos);
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

    float radiusSum =
        a.getRadius() +
        b.getRadius();

    if (distance >= radiusSum)
        return;

    if (distance < 0.001f)
    {
        delta = { 1.f, 0.f };
        distance = 1.f;
    }

    sf::Vector2f normal =
    {
        delta.x / distance,
        delta.y / distance
    };

    float penetration =
        radiusSum - distance;

    posA.x += normal.x * penetration / 2.f;
    posA.y += normal.y * penetration / 2.f;

    posB.x -= normal.x * penetration / 2.f;
    posB.y -= normal.y * penetration / 2.f;

    sf::Vector2f relative =
    {
        velA.x - velB.x,
        velA.y - velB.y
    };

    float velocityAlongNormal =
        relative.x * normal.x +
        relative.y * normal.y;

    if (velocityAlongNormal < 0.f)
    {
        constexpr float restitution = 1.f;

        float impulse =
            -(1.f + restitution) *
            velocityAlongNormal /
            2.f;

        velA.x += impulse * normal.x;
        velA.y += impulse * normal.y;

        velB.x -= impulse * normal.x;
        velB.y -= impulse * normal.y;

        a.setVelocity(velA);
        b.setVelocity(velB);
    }

    a.setPosition(posA);
    b.setPosition(posB);
}

bool Collision::isBallBelowBottom(
    const Ball& ball,
    float bottomY)
{
    return
        ball.getPosition().y -
        ball.getRadius() >
        bottomY;
}
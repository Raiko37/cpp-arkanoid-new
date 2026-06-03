#include "Ball.h"

#include "Constants.h"
#include "Utils.h"

#include <cmath>

const float Ball::MAX_SPEED = BALL_MAX_SPEED;
const float Ball::MIN_SPEED = BALL_MIN_SPEED;

Ball::Ball(
    const sf::Vector2f& position,
    const sf::Vector2f& velocity,
    float radius)
    :
    velocity(velocity),
    radius(radius),
    sticky(false)
{
    shape.setRadius(radius);

    shape.setOrigin(radius, radius);

    shape.setPosition(position);

    shape.setFillColor(sf::Color::White);

    shape.setOutlineColor(sf::Color::Red);

    shape.setOutlineThickness(2.f);
}

void Ball::update(float dt)
{
    if (sticky)
        return;

    shape.move(velocity * dt);
}

void Ball::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

void Ball::setVelocity(const sf::Vector2f& newVel)
{
    velocity = newVel;
}

sf::Vector2f Ball::getVelocity() const
{
    return velocity;
}

sf::FloatRect Ball::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Ball::getPosition() const
{
    return shape.getPosition();
}

void Ball::setPosition(const sf::Vector2f& pos)
{
    shape.setPosition(pos);
}

void Ball::increaseSpeed(float factor)
{
    float speed =
        std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y);

    speed *= factor;

    if (speed > MAX_SPEED)
        speed = MAX_SPEED;

    sf::Vector2f dir = normalize(velocity);

    velocity = dir * speed;
}

void Ball::decreaseSpeed(float factor)
{
    float speed =
        std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y);

    speed *= factor;

    if (speed < MIN_SPEED)
        speed = MIN_SPEED;

    sf::Vector2f dir = normalize(velocity);

    velocity = dir * speed;
}

bool Ball::isSticky() const
{
    return sticky;
}

void Ball::setSticky(bool value)
{
    sticky = value;
}

float Ball::getRadius() const
{
    return radius;
}
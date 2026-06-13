#include "Ball.h"

#include "Constants.h"
#include "Utils.h"

#include <algorithm>
#include <cmath>

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

    if (speed < 0.0001f)
        return;

    speed = std::min(
        speed * factor,
        BALL_MAX_SPEED);

    sf::Vector2f direction =
        normalize(velocity);

    velocity = direction * speed;
}

void Ball::decreaseSpeed(float factor)
{
    float speed =
        std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y);

    if (speed < 0.0001f)
        return;

    speed = std::max(
        speed * factor,
        BALL_MIN_SPEED);

    sf::Vector2f direction =
        normalize(velocity);

    velocity = direction * speed;
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
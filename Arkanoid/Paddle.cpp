#include "Paddle.h"

#include "Constants.h"

#include <algorithm>

Paddle::Paddle(
    float startX,
    float startY,
    float initialWidth,
    float height)
    :
    speed(PADDLE_SPEED),
    baseWidth(initialWidth)
{
    shape.setSize({ initialWidth, height });
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(
        initialWidth / 2.f,
        height / 2.f);
    shape.setPosition(startX, startY);
}

void Paddle::update(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveLeft(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveRight(dt);
}

void Paddle::moveLeft(float dt)
{
    shape.move(-speed * dt, 0.f);

    if (shape.getGlobalBounds().left < 0.f)
    {
        shape.setPosition(
            shape.getSize().x / 2.f,
            shape.getPosition().y);
    }
}

void Paddle::moveRight(float dt)
{
    shape.move(speed * dt, 0.f);

    float right =
        shape.getGlobalBounds().left +
        shape.getGlobalBounds().width;

    if (right > WINDOW_WIDTH)
    {
        shape.setPosition(
            WINDOW_WIDTH - shape.getSize().x / 2.f,
            shape.getPosition().y);
    }
}

void Paddle::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

void Paddle::resize(float newWidth)
{
    newWidth = std::clamp(
        newWidth,
        PADDLE_MIN_WIDTH,
        PADDLE_MAX_WIDTH);

    float height = shape.getSize().y;

    shape.setSize({ newWidth, height });
    shape.setOrigin(
        newWidth / 2.f,
        height / 2.f);
}

sf::FloatRect Paddle::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const
{
    return shape.getPosition();
}

void Paddle::setPosition(const sf::Vector2f& pos)
{
    shape.setPosition(pos);
}

float Paddle::getWidth() const
{
    return shape.getSize().x;
}

void Paddle::resetSize()
{
    resize(baseWidth);
}
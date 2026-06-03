#pragma once

#include <SFML/Graphics.hpp>

class Paddle
{
public:
    Paddle(
        float startX,
        float startY,
        float initialWidth,
        float height
    );

    void update(float dt);

    void draw(sf::RenderWindow& window) const;

    void moveLeft(float dt);

    void moveRight(float dt);

    void resize(float newWidth);

    sf::FloatRect getGlobalBounds() const;

    sf::Vector2f getPosition() const;

    void setPosition(const sf::Vector2f& pos);

    float getWidth() const;

    void resetSize();

private:
    sf::RectangleShape shape;

    float speed;

    float baseWidth;

    static const float MIN_WIDTH;
    static const float MAX_WIDTH;
};
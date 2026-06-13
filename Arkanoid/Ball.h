#pragma once

#include <SFML/Graphics.hpp>

class Ball
{
public:
    Ball(
        const sf::Vector2f& position,
        const sf::Vector2f& velocity,
        float radius
    );

    void update(float dt);

    void draw(sf::RenderWindow& window) const;

    void setVelocity(const sf::Vector2f& newVel);

    sf::Vector2f getVelocity() const;

    sf::FloatRect getGlobalBounds() const;

    sf::Vector2f getPosition() const;

    void setPosition(const sf::Vector2f& pos);

    void increaseSpeed(float factor);

    void decreaseSpeed(float factor);

    bool isSticky() const;

    void setSticky(bool sticky);

    float getRadius() const;

private:
    sf::CircleShape shape;

    sf::Vector2f velocity;

    float radius;

    bool sticky;
};
#pragma once

#include <SFML/Graphics.hpp>
#include "Block.h"

class Bonus
{
public:
    Bonus(
        const sf::Vector2f& position,
        BonusType type
    );

    void update(float dt);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const;

    BonusType getType() const;

    bool isActive() const;

    void deactivate();

private:
    sf::RectangleShape shape;

    BonusType type;

    float fallSpeed;

    bool active;
};
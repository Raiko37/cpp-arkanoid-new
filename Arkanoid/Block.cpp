#include "Block.h"
#include "Ball.h"

Block::Block(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    BlockType type,
    int health,
    BonusType bonus)
    :
    type(type),
    health(health),
    containedBonus(bonus)
{
    shape.setSize(size);

    shape.setPosition(position);

    shape.setFillColor(getColor());

    shape.setOutlineThickness(1.f);

    shape.setOutlineColor(sf::Color::Black);
}

bool Block::handleHit(Ball&)
{
    switch (type)
    {
    case BlockType::Indestructible:
        return false;

    case BlockType::Normal:
        return true;

    case BlockType::BonusDrop:
        return true;

    case BlockType::SpeedIncrease:
        return true;

    case BlockType::Health:

        --health;

        shape.setFillColor(getColor());

        return health <= 0;
    }

    return false;
}

void Block::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::FloatRect Block::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

BlockType Block::getType() const
{
    return type;
}

int Block::getHealth() const
{
    return health;
}

BonusType Block::getBonusType() const
{
    return containedBonus;
}

void Block::setHealth(int h)
{
    health = h;
}

sf::Color Block::getColor() const
{
    switch (type)
    {
    case BlockType::Normal:
        return sf::Color::Green;

    case BlockType::Indestructible:
        return sf::Color(120, 120, 120);

    case BlockType::BonusDrop:
        return sf::Color::Blue;

    case BlockType::SpeedIncrease:
        return sf::Color::Yellow;

    case BlockType::Health:

        if (health >= 3)
            return sf::Color(120, 0, 0);

        if (health == 2)
            return sf::Color(180, 0, 0);

        return sf::Color::Red;
    }

    return sf::Color::White;
}
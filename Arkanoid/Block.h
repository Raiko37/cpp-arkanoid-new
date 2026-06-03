#pragma once

#include <SFML/Graphics.hpp>

class Ball;

enum class BlockType
{
    Normal,
    Indestructible,
    SpeedIncrease,
    BonusDrop,
    Health
};

enum class BonusType
{
    ExpandPaddle,
    ShrinkPaddle,
    IncreaseSpeed,
    DecreaseSpeed,
    StickyPaddle,
    OneShotFloor,
    ExtraBall,
    None
};

class Block
{
public:
    Block(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        BlockType type,
        int health = 1,
        BonusType bonus = BonusType::None
    );

    bool handleHit(Ball& ball);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const;

    BlockType getType() const;

    int getHealth() const;

    BonusType getBonusType() const;

    void setHealth(int h);

private:
    sf::RectangleShape shape;

    BlockType type;

    int health;

    BonusType containedBonus;

    sf::Color getColor() const;
};
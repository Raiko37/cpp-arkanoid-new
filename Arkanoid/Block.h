#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

class Ball;
class Bonus;

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
        int health = 1
    );

    virtual ~Block() = default;

    virtual void resolveCollision(Ball& ball) const;

    virtual bool handleHit(Ball& ball) = 0;

    virtual int getScoreForHit(bool destroyed) const;

    virtual bool isDestructible() const = 0;

    virtual std::unique_ptr<Bonus> createBonus() const;

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const;

    int getHealth() const;

    void setHealth(int h);

protected:
    sf::RectangleShape shape;

    int health;

    virtual sf::Color getColor() const = 0;

    void updateColor();
};

class NormalBlock : public Block
{
public:
    NormalBlock(
        const sf::Vector2f& position,
        const sf::Vector2f& size
    );

    bool handleHit(Ball& ball) override;

    bool isDestructible() const override;

protected:
    sf::Color getColor() const override;
};

class IndestructibleBlock : public Block
{
public:
    IndestructibleBlock(
        const sf::Vector2f& position,
        const sf::Vector2f& size
    );

    bool handleHit(Ball& ball) override;

    int getScoreForHit(bool destroyed) const override;

    bool isDestructible() const override;

protected:
    sf::Color getColor() const override;
};

class SpeedIncreaseBlock : public Block
{
public:
    SpeedIncreaseBlock(
        const sf::Vector2f& position,
        const sf::Vector2f& size
    );

    bool handleHit(Ball& ball) override;

    bool isDestructible() const override;

protected:
    sf::Color getColor() const override;
};

class BonusDropBlock : public Block
{
public:
    BonusDropBlock(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        BonusType bonus
    );

    bool handleHit(Ball& ball) override;

    bool isDestructible() const override;

    std::unique_ptr<Bonus> createBonus() const override;

protected:
    sf::Color getColor() const override;

private:
    BonusType containedBonus;
};

class HealthBlock : public Block
{
public:
    HealthBlock(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        int health
    );

    bool handleHit(Ball& ball) override;

    int getScoreForHit(bool destroyed) const override;

    bool isDestructible() const override;

protected:
    sf::Color getColor() const override;
};

class BlockFactory
{
public:
    static std::unique_ptr<Block> create(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        BlockType type,
        int health = 1,
        BonusType bonus = BonusType::None
    );
};
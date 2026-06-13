#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include "Block.h"

class Game;

class Bonus
{
public:
    Bonus(
        const sf::Vector2f& position,
        const sf::Color& color
    );

    virtual ~Bonus() = default;

    void update(float dt);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const;

    virtual void apply(Game& game) = 0;

    bool isActive() const;

    void deactivate();

protected:
    sf::RectangleShape shape;

    float fallSpeed;

    bool active;
};

class ExpandPaddleBonus : public Bonus
{
public:
    explicit ExpandPaddleBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class ShrinkPaddleBonus : public Bonus
{
public:
    explicit ShrinkPaddleBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class IncreaseSpeedBonus : public Bonus
{
public:
    explicit IncreaseSpeedBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class DecreaseSpeedBonus : public Bonus
{
public:
    explicit DecreaseSpeedBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class StickyPaddleBonus : public Bonus
{
public:
    explicit StickyPaddleBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class OneShotFloorBonus : public Bonus
{
public:
    explicit OneShotFloorBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class ExtraBallBonus : public Bonus
{
public:
    explicit ExtraBallBonus(const sf::Vector2f& position);

    void apply(Game& game) override;
};

class BonusFactory
{
public:
    static std::unique_ptr<Bonus> create(
        const sf::Vector2f& position,
        BonusType type
    );
};
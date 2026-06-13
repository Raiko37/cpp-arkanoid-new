#include "Bonus.h"

#include "Constants.h"
#include "Game.h"

Bonus::Bonus(
    const sf::Vector2f& position,
    const sf::Color& color)
    :
    fallSpeed(BONUS_FALL_SPEED),
    active(true)
{
    shape.setSize({ 20.f, 20.f });
    shape.setPosition(position);
    shape.setFillColor(color);
}

void Bonus::update(float dt)
{
    shape.move(0.f, fallSpeed * dt);
}

void Bonus::draw(sf::RenderWindow& window) const
{
    if (active)
        window.draw(shape);
}

sf::FloatRect Bonus::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

bool Bonus::isActive() const
{
    return active;
}

void Bonus::deactivate()
{
    active = false;
}

ExpandPaddleBonus::ExpandPaddleBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color(255, 215, 0))
{
}

void ExpandPaddleBonus::apply(Game& game)
{
    game.expandPaddle(1.3f);
}

ShrinkPaddleBonus::ShrinkPaddleBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color::Red)
{
}

void ShrinkPaddleBonus::apply(Game& game)
{
    game.shrinkPaddle(0.8f);
}

IncreaseSpeedBonus::IncreaseSpeedBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color::Yellow)
{
}

void IncreaseSpeedBonus::apply(Game& game)
{
    game.changeBallsSpeed(1.2f);
}

DecreaseSpeedBonus::DecreaseSpeedBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color::Cyan)
{
}

void DecreaseSpeedBonus::apply(Game& game)
{
    game.changeBallsSpeed(0.8f);
}

StickyPaddleBonus::StickyPaddleBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color::Magenta)
{
}

void StickyPaddleBonus::apply(Game& game)
{
    game.activateStickyMode();
}

OneShotFloorBonus::OneShotFloorBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color::White)
{
}

void OneShotFloorBonus::apply(Game& game)
{
    game.activateOneShotFloor();
}

ExtraBallBonus::ExtraBallBonus(
    const sf::Vector2f& position)
    :
    Bonus(position, sf::Color(160, 32, 240))
{
}

void ExtraBallBonus::apply(Game& game)
{
    game.spawnExtraBall();
}

std::unique_ptr<Bonus> BonusFactory::create(
    const sf::Vector2f& position,
    BonusType type)
{
    switch (type)
    {
    case BonusType::ExpandPaddle:
        return std::make_unique<ExpandPaddleBonus>(position);

    case BonusType::ShrinkPaddle:
        return std::make_unique<ShrinkPaddleBonus>(position);

    case BonusType::IncreaseSpeed:
        return std::make_unique<IncreaseSpeedBonus>(position);

    case BonusType::DecreaseSpeed:
        return std::make_unique<DecreaseSpeedBonus>(position);

    case BonusType::StickyPaddle:
        return std::make_unique<StickyPaddleBonus>(position);

    case BonusType::OneShotFloor:
        return std::make_unique<OneShotFloorBonus>(position);

    case BonusType::ExtraBall:
        return std::make_unique<ExtraBallBonus>(position);

    case BonusType::None:
        return nullptr;
    }

    return nullptr;
}
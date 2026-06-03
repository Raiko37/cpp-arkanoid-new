#include "Bonus.h"
#include "Constants.h"

Bonus::Bonus(
    const sf::Vector2f& position,
    BonusType type)
    :
    type(type),
    fallSpeed(BONUS_FALL_SPEED),
    active(true)
{
    shape.setSize({ 20.f,20.f });

    shape.setPosition(position);

    switch (type)
    {
    case BonusType::ExpandPaddle:
        shape.setFillColor(sf::Color(255, 215, 0));
        break;

    case BonusType::ShrinkPaddle:
        shape.setFillColor(sf::Color::Red);
        break;

    case BonusType::IncreaseSpeed:
        shape.setFillColor(sf::Color::Yellow);
        break;

    case BonusType::DecreaseSpeed:
        shape.setFillColor(sf::Color::Cyan);
        break;

    case BonusType::StickyPaddle:
        shape.setFillColor(sf::Color::Magenta);
        break;

    case BonusType::OneShotFloor:
        shape.setFillColor(sf::Color::White);
        break;

    case BonusType::ExtraBall:
        shape.setFillColor(sf::Color(160, 32, 240));
        break;

    default:
        shape.setFillColor(sf::Color::Green);
    }
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

BonusType Bonus::getType() const
{
    return type;
}

bool Bonus::isActive() const
{
    return active;
}

void Bonus::deactivate()
{
    active = false;
}
#include "Block.h"

#include "Ball.h"
#include "Bonus.h"
#include "Collision.h"

Block::Block(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    int health)
    :
    health(health)
{
    shape.setSize(size);
    shape.setPosition(position);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::Black);
}

void Block::resolveCollision(Ball& ball) const
{
    Collision::resolveBallBlockCollision(
        ball,
        getGlobalBounds());
}

int Block::getScoreForHit(bool destroyed) const
{
    return destroyed ? 1 : 0;
}

std::unique_ptr<Bonus> Block::createBonus() const
{
    return nullptr;
}

void Block::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

sf::FloatRect Block::getGlobalBounds() const
{
    return shape.getGlobalBounds();
}

int Block::getHealth() const
{
    return health;
}

void Block::setHealth(int h)
{
    health = h;
    updateColor();
}

void Block::updateColor()
{
    shape.setFillColor(getColor());
}

NormalBlock::NormalBlock(
    const sf::Vector2f& position,
    const sf::Vector2f& size)
    :
    Block(position, size, 1)
{
    updateColor();
}

bool NormalBlock::handleHit(Ball&)
{
    return true;
}

bool NormalBlock::isDestructible() const
{
    return true;
}

sf::Color NormalBlock::getColor() const
{
    return sf::Color::Green;
}

IndestructibleBlock::IndestructibleBlock(
    const sf::Vector2f& position,
    const sf::Vector2f& size)
    :
    Block(position, size, 1)
{
    updateColor();
}

bool IndestructibleBlock::handleHit(Ball&)
{
    return false;
}

int IndestructibleBlock::getScoreForHit(bool) const
{
    return 0;
}

bool IndestructibleBlock::isDestructible() const
{
    return false;
}

sf::Color IndestructibleBlock::getColor() const
{
    return sf::Color(120, 120, 120);
}

SpeedIncreaseBlock::SpeedIncreaseBlock(
    const sf::Vector2f& position,
    const sf::Vector2f& size)
    :
    Block(position, size, 1)
{
    updateColor();
}

bool SpeedIncreaseBlock::handleHit(Ball& ball)
{
    ball.increaseSpeed(1.03f);

    return true;
}

bool SpeedIncreaseBlock::isDestructible() const
{
    return true;
}

sf::Color SpeedIncreaseBlock::getColor() const
{
    return sf::Color::Yellow;
}

BonusDropBlock::BonusDropBlock(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    BonusType bonus)
    :
    Block(position, size, 1),
    containedBonus(bonus)
{
    updateColor();
}

bool BonusDropBlock::handleHit(Ball&)
{
    return true;
}

bool BonusDropBlock::isDestructible() const
{
    return true;
}

std::unique_ptr<Bonus> BonusDropBlock::createBonus() const
{
    sf::FloatRect rect =
        getGlobalBounds();

    return BonusFactory::create(
        {
            rect.left + rect.width / 2.f - 10.f,
            rect.top + rect.height / 2.f - 10.f
        },
        containedBonus);
}

sf::Color BonusDropBlock::getColor() const
{
    return sf::Color::Blue;
}

HealthBlock::HealthBlock(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    int health)
    :
    Block(position, size, health)
{
    if (this->health < 1)
        this->health = 1;

    updateColor();
}

bool HealthBlock::handleHit(Ball&)
{
    --health;
    updateColor();

    return health <= 0;
}

int HealthBlock::getScoreForHit(bool) const
{
    return 1;
}

bool HealthBlock::isDestructible() const
{
    return true;
}

sf::Color HealthBlock::getColor() const
{
    if (health >= 3)
        return sf::Color(120, 0, 0);

    if (health == 2)
        return sf::Color(180, 0, 0);

    return sf::Color::Red;
}

std::unique_ptr<Block> BlockFactory::create(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    BlockType type,
    int health,
    BonusType bonus)
{
    switch (type)
    {
    case BlockType::Normal:
        return std::make_unique<NormalBlock>(
            position,
            size);

    case BlockType::Indestructible:
        return std::make_unique<IndestructibleBlock>(
            position,
            size);

    case BlockType::SpeedIncrease:
        return std::make_unique<SpeedIncreaseBlock>(
            position,
            size);

    case BlockType::BonusDrop:
        return std::make_unique<BonusDropBlock>(
            position,
            size,
            bonus);

    case BlockType::Health:
        return std::make_unique<HealthBlock>(
            position,
            size,
            health);
    }

    return std::make_unique<NormalBlock>(
        position,
        size);
}
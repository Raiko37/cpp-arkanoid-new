#include "Game.h"

#include "Constants.h"
#include "Collision.h"
#include "Utils.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <string>

Game::Game()
    :
    window(
        sf::VideoMode(
            WINDOW_WIDTH,
            WINDOW_HEIGHT),
        "Arkanoid"),

    paddle(
        WINDOW_WIDTH / 2.f,
        WINDOW_HEIGHT - 40.f,
        PADDLE_INIT_WIDTH,
        PADDLE_HEIGHT),

    score(0),
    lives(INIT_LIVES),
    gameOver(false),
    oneShotFloorActive(false),
    stickyMode(false),
    selectedLevel(LevelType::Classic),
    levelSelected(false)
{
    window.setFramerateLimit(60);

    loadFont(
        font,
        "assets/arial.ttf");

    scoreText.setFont(font);
    livesText.setFont(font);
    gameOverText.setFont(font);

    scoreText.setCharacterSize(24);
    livesText.setCharacterSize(24);
    gameOverText.setCharacterSize(40);

    scoreText.setPosition(10.f, 10.f);

    livesText.setPosition(
        WINDOW_WIDTH - 150.f,
        10.f);

    scoreText.setString("Score: 0");
    livesText.setString(
        "Lives: " +
        std::to_string(lives));

    gameOverText.setString("GAME OVER");

    gameOverText.setPosition(
        250.f,
        250.f);

    resetBallsOnPaddle();
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt =
            clock.restart().asSeconds();

        processInput();

        if (!gameOver && levelSelected)
            update(dt);

        render();
    }
}

void Game::processInput()
{
    sf::Event event;

    if (!levelSelected)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type != sf::Event::KeyPressed)
                continue;

            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
            else if (event.key.code == sf::Keyboard::Num1)
            {
                selectedLevel = LevelType::Classic;
                levelSelected = true;
                spawnLevel();
                resetBallsOnPaddle();
            }
            else if (event.key.code == sf::Keyboard::Num2)
            {
                selectedLevel = LevelType::Fortress;
                levelSelected = true;
                spawnLevel();
                resetBallsOnPaddle();
            }
            else if (event.key.code == sf::Keyboard::Num3)
            {
                selectedLevel = LevelType::Maze;
                levelSelected = true;
                spawnLevel();
                resetBallsOnPaddle();
            }
        }

        return;
    }

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type != sf::Event::KeyPressed)
            continue;

        if (event.key.code == sf::Keyboard::Escape)
        {
            window.close();
        }
        else if (event.key.code == sf::Keyboard::Space)
        {
            for (auto& ball : balls)
            {
                if (!ball.isSticky())
                    continue;

                ball.setSticky(false);

                sf::Vector2f velocity =
                    ball.getVelocity();

                if (
                    std::abs(velocity.x) < 0.001f &&
                    std::abs(velocity.y) < 0.001f)
                {
                    ball.setVelocity(
                        {
                            BALL_INIT_SPEED,
                            -BALL_INIT_SPEED
                        });
                }
            }
        }
    }
}

BonusType Game::getRandomBonusType() const
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    static std::uniform_int_distribution<int> distribution(
        0,
        static_cast<int>(BonusType::ExtraBall));

    return static_cast<BonusType>(
        distribution(generator));
}

void Game::spawnLevel()
{
    blocks.clear();
    activeBonuses.clear();

    const sf::Vector2f blockSize(
        BLOCK_WIDTH,
        BLOCK_HEIGHT);

    constexpr float startX = 30.f;
    constexpr float startY = 40.f;
    constexpr float stepX = 64.f;
    constexpr float stepY = 29.f;

    switch (selectedLevel)
    {
    case LevelType::Classic:
    {
        for (int row = 0; row < BLOCK_ROWS; ++row)
        {
            for (int col = 0; col < BLOCK_COLS; ++col)
            {
                sf::Vector2f position(
                    startX + col * stepX,
                    startY + row * stepY);

                if (row < 2)
                {
                    blocks.push_back(
                        BlockFactory::create(
                            position,
                            blockSize,
                            BlockType::Indestructible));
                }
                else if (row < 4)
                {
                    blocks.push_back(
                        BlockFactory::create(
                            position,
                            blockSize,
                            BlockType::Normal));
                }
                else if (row == 4)
                {
                    blocks.push_back(
                        BlockFactory::create(
                            position,
                            blockSize,
                            BlockType::BonusDrop,
                            1,
                            getRandomBonusType()));
                }
                else if (row == 5)
                {
                    blocks.push_back(
                        BlockFactory::create(
                            position,
                            blockSize,
                            BlockType::Health,
                            3));
                }
                else
                {
                    blocks.push_back(
                        BlockFactory::create(
                            position,
                            blockSize,
                            BlockType::SpeedIncrease));
                }
            }
        }

        break;
    }

    case LevelType::Fortress:
    {
        for (int row = 0; row < BLOCK_ROWS; ++row)
        {
            for (int col = 0; col < BLOCK_COLS; ++col)
            {
                bool place = false;
                BlockType type = BlockType::Normal;
                int health = 1;
                BonusType bonus = BonusType::None;

                if (
                    (row == 0 && col != 5 && col != 6) ||
                    (row == 7 && col != 5 && col != 6) ||
                    col == 0 ||
                    col == 11)
                {
                    place = true;
                    type = BlockType::Indestructible;
                }

                if (
                    row >= 2 &&
                    row <= 5 &&
                    col >= 4 &&
                    col <= 7)
                {
                    place = true;
                    type = BlockType::Health;
                    health = 3;
                }

                if (
                    (row == 2 || row == 5) &&
                    (col == 2 || col == 9))
                {
                    place = true;
                    type = BlockType::BonusDrop;
                    bonus = getRandomBonusType();
                }

                if (!place)
                    continue;

                sf::Vector2f position(
                    startX + col * stepX,
                    startY + row * stepY);

                blocks.push_back(
                    BlockFactory::create(
                        position,
                        blockSize,
                        type,
                        health,
                        bonus));
            }
        }

        break;
    }

    case LevelType::Maze:
    {
        int maze[BLOCK_ROWS][BLOCK_COLS] =
        {
            {1,1,1,1,0,0,0,0,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {0,0,1,1,0,0,0,0,1,1,0,0},
            {0,0,1,0,0,1,1,0,0,1,0,0},
            {0,0,1,0,0,1,1,0,0,1,0,0},
            {0,0,1,1,0,0,0,0,1,1,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,0,0,0,0,1,1,1,1}
        };

        for (int row = 0; row < BLOCK_ROWS; ++row)
        {
            for (int col = 0; col < BLOCK_COLS; ++col)
            {
                if (!maze[row][col])
                    continue;

                sf::Vector2f position(
                    startX + col * stepX,
                    startY + row * stepY);

                BlockType type = BlockType::Normal;
                int health = 1;
                BonusType bonus = BonusType::None;

                if (
                    row == 0 ||
                    row == 7 ||
                    col == 0 ||
                    col == 11)
                {
                    type = BlockType::Indestructible;
                }
                else
                {
                    int variant =
                        static_cast<int>(
                            randomFloat(0.f, 3.f));

                    if (variant == 0)
                    {
                        type = BlockType::Normal;
                    }
                    else if (variant == 1)
                    {
                        type = BlockType::BonusDrop;
                        bonus = getRandomBonusType();
                    }
                    else
                    {
                        type = BlockType::Health;
                        health =
                            2 +
                            static_cast<int>(
                                randomFloat(0.f, 2.f));
                    }
                }

                blocks.push_back(
                    BlockFactory::create(
                        position,
                        blockSize,
                        type,
                        health,
                        bonus));
            }
        }

        break;
    }
    }
}

void Game::addBall(
    const sf::Vector2f& position,
    const sf::Vector2f& velocity)
{
    balls.emplace_back(
        position,
        velocity,
        BALL_RADIUS);
}

void Game::removeBall(int index)
{
    if (index < 0)
        return;

    if (index >= static_cast<int>(balls.size()))
        return;

    balls.erase(
        balls.begin() + index);
}

void Game::resetBallsOnPaddle()
{
    balls.clear();

    addBall(
        {
            paddle.getPosition().x,
            paddle.getPosition().y - 20.f
        },
        {
            BALL_INIT_SPEED,
            -BALL_INIT_SPEED
        });

        balls.back().setSticky(true);
}

void Game::update(float dt)
{
    paddle.update(dt);

    for (auto& ball : balls)
    {
        if (ball.isSticky())
        {
            ball.setPosition(
                {
                    paddle.getPosition().x,
                    paddle.getPosition().y - 20.f
                });

            continue;
        }

        ball.update(dt);
    }

    for (auto& bonus : activeBonuses)
    {
        if (!bonus->isActive())
            continue;

        bonus->update(dt);

        if (bonus->getGlobalBounds().top > WINDOW_HEIGHT)
            bonus->deactivate();
    }

    checkBonusesCollision();

    for (auto& ball : balls)
    {
        if (ball.isSticky())
            continue;

        sf::Vector2f position =
            ball.getPosition();

        sf::Vector2f velocity =
            ball.getVelocity();

        float radius =
            ball.getRadius();

        if (position.x - radius < 0.f)
        {
            position.x = radius;
            velocity.x = std::abs(velocity.x);
        }

        if (position.x + radius > WINDOW_WIDTH)
        {
            position.x = WINDOW_WIDTH - radius;
            velocity.x = -std::abs(velocity.x);
        }

        if (position.y - radius < 0.f)
        {
            position.y = radius;
            velocity.y = std::abs(velocity.y);
        }

        ball.setPosition(position);
        ball.setVelocity(velocity);

        if (!Collision::checkAABB(
            ball.getGlobalBounds(),
            paddle.getGlobalBounds()))
        {
            continue;
        }

        if (stickyMode)
        {
            ball.setSticky(true);
            ball.setVelocity({ 0.f, 0.f });
            stickyMode = false;
        }
        else if (ball.getVelocity().y > 0.f)
        {
            sf::Vector2f currentVelocity =
                ball.getVelocity();

            float speed =
                std::sqrt(
                    currentVelocity.x * currentVelocity.x +
                    currentVelocity.y * currentVelocity.y);

            if (speed < BALL_MIN_SPEED)
                speed = BALL_INIT_SPEED;

            float offset =
                (ball.getPosition().x - paddle.getPosition().x) /
                (paddle.getWidth() / 2.f);

            offset = std::clamp(
                offset,
                -1.f,
                1.f);

            sf::Vector2f direction =
                normalize({ offset, -1.f });

            ball.setVelocity(direction * speed);

            ball.setPosition(
                {
                    ball.getPosition().x,
                    paddle.getGlobalBounds().top -
                    ball.getRadius() -
                    0.1f
                });
        }
    }

    std::vector<size_t> blocksToRemove;

    for (auto& ball : balls)
    {
        if (ball.isSticky())
            continue;

        for (size_t i = 0; i < blocks.size(); ++i)
        {
            if (
                std::find(
                    blocksToRemove.begin(),
                    blocksToRemove.end(),
                    i) != blocksToRemove.end())
            {
                continue;
            }

            if (!Collision::checkAABB(
                ball.getGlobalBounds(),
                blocks[i]->getGlobalBounds()))
            {
                continue;
            }

            blocks[i]->resolveCollision(ball);

            bool destroyed =
                blocks[i]->handleHit(ball);

            score +=
                blocks[i]->getScoreForHit(destroyed);

            if (destroyed)
            {
                std::unique_ptr<Bonus> bonus =
                    blocks[i]->createBonus();

                if (bonus)
                    activeBonuses.push_back(std::move(bonus));

                blocksToRemove.push_back(i);
            }

            break;
        }
    }

    std::sort(
        blocksToRemove.begin(),
        blocksToRemove.end());

    blocksToRemove.erase(
        std::unique(
            blocksToRemove.begin(),
            blocksToRemove.end()),
        blocksToRemove.end());

    for (
        int i = static_cast<int>(blocksToRemove.size()) - 1;
        i >= 0;
        --i)
    {
        blocks.erase(
            blocks.begin() + blocksToRemove[i]);
    }

    for (size_t i = 0; i < balls.size(); ++i)
    {
        for (size_t j = i + 1; j < balls.size(); ++j)
        {
            if (
                balls[i].isSticky() ||
                balls[j].isSticky())
            {
                continue;
            }

            Collision::resolveBallBallCollision(
                balls[i],
                balls[j]);
        }
    }

    for (
        int i = static_cast<int>(balls.size()) - 1;
        i >= 0;
        --i)
    {
        if (!Collision::isBallBelowBottom(
            balls[i],
            WINDOW_HEIGHT))
        {
            continue;
        }

        if (oneShotFloorActive)
        {
            oneShotFloorActive = false;

            sf::Vector2f velocity =
                balls[i].getVelocity();

            velocity.y = -std::abs(velocity.y);

            balls[i].setVelocity(velocity);

            balls[i].setPosition(
                {
                    balls[i].getPosition().x,
                    WINDOW_HEIGHT - balls[i].getRadius() - 1.f
                });

            continue;
        }

        removeBall(i);

        if (balls.empty())
        {
            --lives;

            paddle.resize(
                paddle.getWidth() * 0.8f);
        }
    }

    if (balls.empty())
    {
        if (lives <= 0)
        {
            gameOver = true;
        }
        else
        {
            resetBallsOnPaddle();
        }
    }

    bool win = true;

    for (const auto& block : blocks)
    {
        if (block->isDestructible())
        {
            win = false;
            break;
        }
    }

    if (win)
    {
        spawnLevel();
        stickyMode = false;
        oneShotFloorActive = false;
        resetBallsOnPaddle();
    }

    scoreText.setString(
        "Score: " +
        std::to_string(score));

    livesText.setString(
        "Lives: " +
        std::to_string(lives));
}

void Game::checkBonusesCollision()
{
    for (auto& bonus : activeBonuses)
    {
        if (!bonus->isActive())
            continue;

        if (Collision::checkAABB(
            bonus->getGlobalBounds(),
            paddle.getGlobalBounds()))
        {
            bonus->apply(*this);
            bonus->deactivate();
        }
    }

    activeBonuses.erase(
        std::remove_if(
            activeBonuses.begin(),
            activeBonuses.end(),
            [](const std::unique_ptr<Bonus>& bonus)
            {
                return !bonus->isActive();
            }),
        activeBonuses.end());
}

void Game::expandPaddle(float factor)
{
    paddle.resize(
        paddle.getWidth() * factor);
}

void Game::shrinkPaddle(float factor)
{
    paddle.resize(
        paddle.getWidth() * factor);
}

void Game::changeBallsSpeed(float factor)
{
    for (auto& ball : balls)
    {
        if (factor >= 1.f)
            ball.increaseSpeed(factor);
        else
            ball.decreaseSpeed(factor);
    }
}

void Game::activateStickyMode()
{
    stickyMode = true;
}

void Game::activateOneShotFloor()
{
    oneShotFloorActive = true;
}

void Game::spawnExtraBall()
{
    sf::Vector2f velocity(
        randomFloat(-250.f, 250.f),
        randomFloat(-300.f, -200.f));

    if (std::abs(velocity.x) < 80.f)
    {
        if (velocity.x < 0.f)
            velocity.x = -80.f;
        else
            velocity.x = 80.f;
    }

    addBall(
        {
            paddle.getPosition().x,
            paddle.getPosition().y - 25.f
        },
        velocity);
}

void Game::render()
{
    window.clear(sf::Color::Black);

    if (!levelSelected)
    {
        sf::Text text;

        text.setFont(font);
        text.setCharacterSize(30);

        text.setString(
            "ARKANOID\n\n"
            "1 - Classic\n"
            "2 - Fortress\n"
            "3 - Maze");

        text.setPosition(220.f, 180.f);

        window.draw(text);
        window.display();

        return;
    }

    paddle.draw(window);

    for (const auto& ball : balls)
    {
        ball.draw(window);
    }

    for (const auto& block : blocks)
    {
        block->draw(window);
    }

    for (const auto& bonus : activeBonuses)
    {
        bonus->draw(window);
    }

    window.draw(scoreText);
    window.draw(livesText);

    if (gameOver)
    {
        window.draw(gameOverText);
    }

    window.display();
}
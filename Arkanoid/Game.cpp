#include "Game.h"

#include "Constants.h"
#include "Collision.h"
#include "Utils.h"

#include <algorithm>

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
    started(false),
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

    gameOverText.setString("GAME OVER");

    gameOverText.setPosition(
        250.f,
        250.f);

    addBall(
        {
            WINDOW_WIDTH / 2.f,
            WINDOW_HEIGHT - 60.f
        },
        {
            BALL_INIT_SPEED,
            -BALL_INIT_SPEED
        });

        balls[0].setSticky(true);

}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt =
            clock.restart().asSeconds();

        processInput();

        if (!gameOver)
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

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Num1)
                {
                    selectedLevel = LevelType::Classic;
                    levelSelected = true;

                    spawnLevel();
                }

                if (event.key.code == sf::Keyboard::Num2)
                {
                    selectedLevel = LevelType::Fortress;
                    levelSelected = true;

                    spawnLevel();
                }

                if (event.key.code == sf::Keyboard::Num3)
                {
                    selectedLevel = LevelType::Maze;
                    levelSelected = true;

                    spawnLevel();
                }
            }
        }

        return;
    }

    while (window.pollEvent(event))
    {
        if (event.type ==
            sf::Event::Closed)
        {
            window.close();
        }

        if (event.type ==
            sf::Event::KeyPressed)
        {
            if (event.key.code ==
                sf::Keyboard::Escape)
            {
                window.close();
            }

            if (event.key.code ==
                sf::Keyboard::Space)
            {
                started = true;

                for (auto& ball : balls)
                {
                    if (ball.isSticky())
                    {
                        ball.setSticky(false);

                        sf::Vector2f vel =
                            ball.getVelocity();

                        if (vel.x == 0.f &&
                            vel.y == 0.f)
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
    }
}

void Game::spawnLevel()
{
    blocks.clear();

    switch (selectedLevel)
    {
    case LevelType::Classic:
    {
        float startX = 30.f;
        float startY = 40.f;

        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 12; col++)
            {
                sf::Vector2f pos(
                    startX + col * 64.f,
                    startY + row * 29.f);

                if (row < 2)
                {
                    blocks.emplace_back(
                        pos,
                        sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT),
                        BlockType::Indestructible);
                }
                else if (row < 4)
                {
                    blocks.emplace_back(
                        pos,
                        sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT),
                        BlockType::Normal);
                }
                else if (row == 4)
                {
                    blocks.emplace_back(
                        pos,
                        sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT),
                        BlockType::BonusDrop,
                        1,
                        static_cast<BonusType>(
                            rand() % 7));
                }
                else if (row == 5)
                {
                    blocks.emplace_back(
                        pos,
                        sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT),
                        BlockType::Health,
                        3);
                }
                else
                {
                    blocks.emplace_back(
                        pos,
                        sf::Vector2f(BLOCK_WIDTH, BLOCK_HEIGHT),
                        BlockType::SpeedIncrease);
                }
            }
        }

        break;
    } case LevelType::Fortress:
    {
        float startX = 30.f;
        float startY = 40.f;

        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 12; col++)
            {
                bool place = false;
                BlockType type = BlockType::Normal;

                
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
                }

                
                if (
                    (row == 2 || row == 5) &&
                    (col == 2 || col == 9))
                {
                    place = true;
                    type = BlockType::BonusDrop;
                }

                if (!place)
                    continue;

                sf::Vector2f pos(
                    startX + col * 64.f,
                    startY + row * 29.f);

                blocks.emplace_back(
                    pos,
                    sf::Vector2f(
                        BLOCK_WIDTH,
                        BLOCK_HEIGHT),
                    type,
                    3,
                    static_cast<BonusType>(rand() % 7));
            }
        }

        break;
    }case LevelType::Maze:
    {
        int maze[8][12] =
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

        float startX = 30.f;
        float startY = 40.f;

        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 12; col++)
            {
                if (!maze[row][col])
                    continue;

                sf::Vector2f pos(
                    startX + col * 64.f,
                    startY + row * 29.f);

                BlockType type;
                int hp = 1;

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
                    int r = rand() % 3;

                    if (r == 0)
                    {
                        type = BlockType::Normal;
                    }
                    else if (r == 1)
                    {
                        type = BlockType::BonusDrop;
                    }
                    else
                    {
                        type = BlockType::Health;
                        hp = 2 + rand() % 3;
                    }
                }

                blocks.emplace_back(
                    pos,
                    sf::Vector2f(
                        BLOCK_WIDTH,
                        BLOCK_HEIGHT),
                    type,
                    hp,
                    static_cast<BonusType>(
                        rand() % 7));
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
        if (bonus.isActive())
            bonus.update(dt);
    }

    scoreText.setString(
        "Score: " +
        std::to_string(score));

    livesText.setString(
        "Lives: " +
        std::to_string(lives));

        checkBonusesCollision();

    for (auto& ball : balls)
    {
        sf::Vector2f pos = ball.getPosition();
        sf::Vector2f vel = ball.getVelocity();

        if (pos.x - BALL_RADIUS < 0.f)
        {
            vel.x = std::abs(vel.x);
            ball.setVelocity(vel);
        }

        if (pos.x + BALL_RADIUS > WINDOW_WIDTH)
        {
            vel.x = -std::abs(vel.x);
            ball.setVelocity(vel);
        }

        if (pos.y - BALL_RADIUS < 0.f)
        {
            vel.y = std::abs(vel.y);
            ball.setVelocity(vel);
        }

        if (Collision::checkAABB(
            ball.getGlobalBounds(),
            paddle.getGlobalBounds()))
        {
            if (stickyMode)
            {
                ball.setSticky(true);
                ball.setVelocity({ 0.f, 0.f });

                stickyMode = false;
            }
            else
            {
                vel = ball.getVelocity();

                vel.y = -std::abs(vel.y);

                float offset =
                    (ball.getPosition().x -
                     paddle.getPosition().x)
                    /
                    (paddle.getWidth() / 2.f);

                vel.x += offset * 120.f;

                ball.setVelocity(vel);
            }
        }
    }

    std::vector<size_t> blocksToRemove;

    for (auto& ball : balls)
    {
        for (size_t i = 0; i < blocks.size(); ++i)
        {
            if (!Collision::checkAABB(
                ball.getGlobalBounds(),
                blocks[i].getGlobalBounds()))
            {
                continue;
            }

            Collision::resolveBallBlockCollision(
                ball,
                blocks[i].getGlobalBounds());

            bool destroyed =
                blocks[i].handleHit(ball);

            if (blocks[i].getType() ==
                BlockType::SpeedIncrease)
            {
                ball.increaseSpeed(1.03f);
            }

            if (blocks[i].getType() ==
                BlockType::Health)
            {
                ++score;
            }

            if (destroyed)
            {
                ++score;

                if (blocks[i].getType() ==
                    BlockType::BonusDrop)
                {
                    activeBonuses.emplace_back(
                        sf::Vector2f(
                            blocks[i]
                            .getGlobalBounds().left +
                            20.f,

                            blocks[i]
                            .getGlobalBounds().top),
                        blocks[i].getBonusType());
                }

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

    for (int i =
        static_cast<int>(
            blocksToRemove.size()) - 1;
        i >= 0;
        --i)
    {
        blocks.erase(
            blocks.begin() +
            blocksToRemove[i]);
    }

    for (size_t i = 0; i < balls.size(); ++i)
    {
        for (size_t j = i + 1;
            j < balls.size();
            ++j)
        {
            Collision::resolveBallBallCollision(
                balls[i],
                balls[j]);
        }
    }

    for (int i =
        static_cast<int>(balls.size()) - 1;
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

            sf::Vector2f vel =
                balls[i].getVelocity();

            vel.y = -std::abs(vel.y);

            balls[i].setVelocity(vel);

            balls[i].setPosition(
                {
                    balls[i].getPosition().x,
                    WINDOW_HEIGHT - 30.f
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
    }

    bool win = true;

    for (const auto& block : blocks)
    {
        if (block.getType() !=
            BlockType::Indestructible)
        {
            win = false;
            break;
        }
    }

    if (win)
    {
        spawnLevel();

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
}

void Game::checkBonusesCollision()
{
    for (auto& bonus : activeBonuses)
    {
        if (!bonus.isActive())
            continue;

        if (Collision::checkAABB(
            bonus.getGlobalBounds(),
            paddle.getGlobalBounds()))
        {
            applyBonus(
                bonus.getType());

            bonus.deactivate();
        }
    }

    activeBonuses.erase(
        std::remove_if(
            activeBonuses.begin(),
            activeBonuses.end(),
            [](const Bonus& b)
            {
                return !b.isActive();
            }),
        activeBonuses.end());
}
void Game::applyBonus(
    BonusType type)
{
    switch (type)
    {
    case BonusType::ExpandPaddle:

        paddle.resize(
            paddle.getWidth() * 1.3f);
        break;

    case BonusType::ShrinkPaddle:

        paddle.resize(
            paddle.getWidth() * 0.8f);
        break;

    case BonusType::IncreaseSpeed:

        for (auto& ball : balls)
            ball.increaseSpeed(1.2f);

        break;

    case BonusType::DecreaseSpeed:

        for (auto& ball : balls)
            ball.decreaseSpeed(0.8f);

        break;

    case BonusType::StickyPaddle:

        stickyMode = true;
        break;

    case BonusType::OneShotFloor:

        oneShotFloorActive = true;
        break;

    case BonusType::ExtraBall:
    {
        sf::Vector2f velocity(
            randomFloat(-250.f, 250.f),
            randomFloat(-300.f, -200.f));

        addBall(
            {
                paddle.getPosition().x,
                paddle.getPosition().y - 25.f
            },
            velocity);

        break;
    }

    default:
        break;
    }
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
        block.draw(window);
    }

    for (const auto& bonus : activeBonuses)
    {
        bonus.draw(window);
    }

    window.draw(scoreText);

    window.draw(livesText);

    if (gameOver)
    {
        window.draw(gameOverText);
    }

    window.display();
}
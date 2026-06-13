#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

#include "Paddle.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"

enum class LevelType
{
    Classic,
    Fortress,
    Maze
};

class Game
{
public:
    Game();

    void run();

    void expandPaddle(float factor);

    void shrinkPaddle(float factor);

    void changeBallsSpeed(float factor);

    void activateStickyMode();

    void activateOneShotFloor();

    void spawnExtraBall();

private:
    void processInput();

    void update(float dt);

    void render();

    void spawnLevel();

    void checkBonusesCollision();

    void removeBall(int index);

    void addBall(
        const sf::Vector2f& position,
        const sf::Vector2f& velocity
    );

    void resetBallsOnPaddle();

    BonusType getRandomBonusType() const;

private:
    sf::RenderWindow window;

    Paddle paddle;

    std::vector<Ball> balls;

    std::vector<std::unique_ptr<Block>> blocks;

    std::vector<std::unique_ptr<Bonus>> activeBonuses;

    int score;

    int lives;

    bool gameOver;

    bool oneShotFloorActive;

    bool stickyMode;

    LevelType selectedLevel;

    bool levelSelected;

    sf::Font font;

    sf::Text scoreText;

    sf::Text livesText;

    sf::Text gameOverText;
};
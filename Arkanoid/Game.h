#pragma once

#include <SFML/Graphics.hpp>

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

private:
    LevelType selectedLevel;

    bool levelSelected;


    void processInput();

    void update(float dt);

    void render();

    void spawnLevel();

    void checkBonusesCollision();

    void applyBonus(BonusType type);

    void removeBall(int index);

    void addBall(
        const sf::Vector2f& position,
        const sf::Vector2f& velocity);

private:

    sf::RenderWindow window;

    Paddle paddle;

    std::vector<Ball> balls;

    std::vector<Block> blocks;

    std::vector<Bonus> activeBonuses;

    int score;

    int lives;

    bool gameOver;

    bool oneShotFloorActive;

    bool stickyMode;

    bool started;

    sf::Font font;

    sf::Text scoreText;

    sf::Text livesText;

    sf::Text gameOverText;
};
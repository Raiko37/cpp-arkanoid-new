#include "Utils.h"

#include <random>
#include <cmath>

float randomFloat(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> dist(min, max);

    return dist(gen);
}

sf::Vector2f normalize(const sf::Vector2f& v)
{
    float len = std::sqrt(v.x * v.x + v.y * v.y);

    if (len < 0.0001f)
        return { 0.f, 0.f };

    return { v.x / len, v.y / len };
}

bool loadFont(sf::Font& font, const std::string& path)
{
    return font.loadFromFile(path);
}
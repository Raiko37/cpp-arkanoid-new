#pragma once

#include <SFML/Graphics.hpp>

float randomFloat(float min, float max);

sf::Vector2f normalize(const sf::Vector2f& v);

bool loadFont(sf::Font& font, const std::string& path);
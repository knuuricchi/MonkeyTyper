#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// Deklaracje funkcji
void spawnWord(const sf::Font& font, const sf::RenderWindow& window);
void runGame(sf::RenderWindow& window, const std::string& speedSetting, const std::string& frequencySetting);
void showPostGameMenu(sf::RenderWindow& window);
void playRandomGameMusic(int volume);

void saveScoreboard(sf::RenderWindow& window);
void loadScoreboard(const std::string& filename);
void showPostGameMenu(sf::RenderWindow& window);

#pragma once

#include <SFML/Graphics.hpp>
#include <string>

void spawnWord(const sf::Font& font, const sf::RenderWindow& window);
void runGame(sf::RenderWindow& window, std::string speedSetting, std::string frequencySetting, int duration);
void showPostGameMenu(sf::RenderWindow& window);
void playRandomGameMusic(int volume);

void saveScoreboard(sf::RenderWindow& window);
void loadScoreboard(const std::string& filename);
void filterScoreboard(sf::RenderWindow& window);
void showPostGameMenu(sf::RenderWindow& window);

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include "globals.hpp"

// Deklaracje funkcji obsługujących menu
void initializeMainMenu(std::vector<sf::Text>& mainMenuTexts, sf::Font& font, int& selectedMainOption);
void handleMainMenuInput(sf::RenderWindow& window, MenuState& currentMenu, std::vector<sf::Text>& mainMenuTexts, int& selectedMainOption);
void drawMainMenu(sf::RenderWindow& window, const sf::Text& title, const std::vector<sf::Text>& mainMenuTexts);

void handleSettingsMenuInput(sf::RenderWindow& window, MenuState& currentMenu, int& selectedSettingsOption);
void drawSettingsMenu(sf::RenderWindow& window, const std::vector<std::string>& settingsOptions, int selectedSettingsOption);

void handleGameSettingsMenuInput(sf::RenderWindow& window, MenuState& currentMenu, int& selectedGameSettingsOption);
void drawGameSettingsMenu(sf::RenderWindow& window, const std::vector<std::string>& gameSettingsOptions, int selectedGameSettingsOption);
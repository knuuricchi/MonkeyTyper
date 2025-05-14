#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <thread>

void loadIcon(sf::RenderWindow& window, const std::string& filename);
void loadFont(const std::string& filename);
void loadWordsFromFile(const std::string& filename);
void loadAudioEffects();
void loadPostGameMuisc(sf::Music& music, const std::string& filename, int volume);
void loadMusic(sf::Music& music, const std::string& filename, int volume);
void showLoadingScreen(sf::RenderWindow& window, const sf::Font& font);
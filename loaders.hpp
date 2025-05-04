#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

void loadIcon(sf::RenderWindow& window, const std::string& filename);
void loadFont(const std::string& filename);
void loadWordsFromFile(const std::string& filename);
void loadAudioEffects();
void loadMusic(sf::Music& music, const std::string& filename, int volume);

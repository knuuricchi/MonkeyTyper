#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

// Enum dla stanu menu
enum class MenuState { MAIN_MENU, SETTINGS, GAME_SETTINGS, GAME, POST_GAME };

// Słowa do rozgrywki
extern sf::Clock colorChangeClock;
extern int currentColorIndex;
extern std::vector<std::string> words;
extern std::vector<sf::Text> activeWords;
extern std::string currentInput;
extern int score;
extern float wordSpeed;
extern float spawnChance;

extern sf::Clock gameClock;

// Czcionki
extern sf::Font font;
extern std::vector<std::string> fontOptions;
extern std::vector<std::string> fontNames;
extern int currentFontIndex;

// Muzyka i dźwięki
extern sf::Music gameMusic;
extern sf::Music lobbyMusic;
extern std::string lobbyMusicFiles;
extern std::vector<std::string> gameMusicFiles;
extern int audioVolume;

// Efekty dźwiękowe
extern sf::SoundBuffer menuMoveBuffer;
extern sf::Sound menuMoveSound;
extern sf::SoundBuffer menuSelectBuffer;
extern sf::Sound menuSelectSound;
extern sf::SoundBuffer countdownBuffer;
extern sf::Sound countdownSound;

// Rozmiar wyświetlanych słów
extern int wordSize;
extern std::vector<int> wordSizeOptions;
extern int currentWordSizeIndex;

// Wyniki rozgrywki
struct ScoreEntry {
    std::string playerName;
    int score;
};
extern std::vector<ScoreEntry> scoreboard;

// Opcje menu, ustawień, rozgrywki oraz zmienne pomocnicze w menu
extern std::vector<std::string> mainMenuOptions;

// Dla ustawień ogólnych
extern std::vector<std::string> settingsOptions;
extern std::vector<sf::Vector2u> resolutions;
extern std::vector<int> fpsOptions;
extern int currentResolutionIndex;
extern int currentFpsIndex;

// Dla ustawień gry
extern std::vector<std::string> gameSettingsOptions;
extern std::vector<std::string> speedOptions;
extern std::vector<std::string> frequencyOptions;
extern std::vector<std::string> presets;
extern int currentSpeedIndex;
extern int currentFrequencyIndex;
extern int currentPresetIndex;
extern std::string gameSpeed;
extern std::string gameFrequency;
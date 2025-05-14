#include "globals.hpp"

// Słowa do rozgrywki
sf::Clock colorChangeClock;
int currentColorIndex = 0;
std::vector<std::string> words;
std::vector<sf::Text> activeWords;
std::string currentInput;
int score = 0;
float wordSpeed = 1.0f;
float spawnChance = 0.5f;

sf::Clock gameClock;

// Czcionki
sf::Font font;
std::vector<std::string> fontOptions = {
    "assets/fonts/BrokenConsole.ttf",
    "assets/fonts/Minecraft.ttf",
    "assets/fonts/ARCADECLASSIC.ttf",
    "assets/fonts/ScriptScreen.ttf"
};
std::vector<std::string> fontNames = {
    "BrokenConsole",
    "Minecraft",
    "ARCADECLASSIC",
    "ScriptScreen"
};
int currentFontIndex = 0;

// Muzyka i dźwięki
sf::Music gameMusic;
sf::Music lobbyMusic;
sf::Music postGameMusic;
std::string lobbyMusicFiles = "assets/sounds/lobby_music.wav";
std::vector<std::string> gameMusicFiles = {
    "assets/sounds/game_music1.wav",
    "assets/sounds/game_music2.wav",
    "assets/sounds/game_music3.wav"
};
std::string postGameMusicFiles = "assets/sounds/post_lobby.wav";

int audioVolume = 5;

// Efekty dźwiękowe
sf::SoundBuffer menuMoveBuffer;
sf::Sound menuMoveSound;
sf::SoundBuffer menuSelectBuffer;
sf::Sound menuSelectSound;
sf::SoundBuffer countdownBuffer;
sf::Sound countdownSound;

// Rozmiar wyświetlanych słów
int wordSize = 30;
std::vector<int> wordSizeOptions = {20, 30, 40, 50};
int currentWordSizeIndex = 1;

// Wyniki rozgrywki
std::vector<ScoreEntry> scoreboard;

// Opcje menu, ustawień, rozgrywki oraz zmienne pomocnicze w menu
std::vector<std::string> mainMenuOptions = {"GRAJ", "USTAWIENIA", "WYJSCIE"};

// Dla ustawień ogólnych
std::vector<std::string> settingsOptions = {
    "Rozdzielczosc: 900x660",
    "FPS: 60",
    "Czcionka: ",
    "Glosnosc: 5/10",
    "Powrot"
};
std::vector<sf::Vector2u> resolutions = {{900u, 660u}, {1024u, 768u}, {1280u, 720u}, {1300u, 1024u}};
std::vector<int> fpsOptions = {30, 60, 120};
int currentResolutionIndex = 0;
int currentFpsIndex = 1;

// Dla ustawień gry
std::vector<std::string> gameSettingsOptions = {
    "Rozpocznij",
    "Szybkosc slow: Normal",
    "Czestotliwosc: Normal",
    "Rozmiar slow: 30",
    "Preset: Medium",
    "Powrot"
};
std::vector<std::string> speedOptions = {"Slow", "Normal", "Fast", "ULTRA FAST", "MONKEY SPEED!!!"};
std::vector<std::string> frequencyOptions = {"Low", "Normal", "High", "MONKEY TYPE-LIKE!!!"};
std::vector<std::string> presets = {"Easy", "Medium", "Hard", "mmOnkEy"};
int currentSpeedIndex = 1;
int currentFrequencyIndex = 1;
int currentPresetIndex = 1;
std::string gameSpeed = "Normal";
std::string gameFrequency = "Normal";
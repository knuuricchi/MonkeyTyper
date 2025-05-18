#include "globals.hpp"
#include <fstream>

void saveGame() {
    std::fstream file("savegame.txt");
    if (file.is_open()) {
        file << score << std::endl;
        file << currentInput << std::endl;
        file << wordSpeed << std::endl;
        file << spawnChance << std::endl;
        file.close();
    }
}

void loadGame() {
    std::fstream file("savegame.txt");
    if (file.is_open()) {
        file >> score;
        file.ignore();
        std::getline(file, currentInput);
        file >> wordSpeed;
        file >> spawnChance;
        file.close();
    }
}

sf::Clock colorChangeClock;
int currentColorIndex = 0;
std::vector<std::string> words;
std::vector<sf::Text> activeWords;
std::string currentInput;
int score = 0;
float wordSpeed = 1.0f;
float spawnChance = 0.5f;

sf::Clock gameClock;

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

sf::SoundBuffer menuMoveBuffer;
sf::Sound menuMoveSound;
sf::SoundBuffer menuSelectBuffer;
sf::Sound menuSelectSound;
sf::SoundBuffer countdownBuffer;
sf::Sound countdownSound;

int wordSize = 30;
std::vector<int> wordSizeOptions = {20, 30, 40, 50};
int currentWordSizeIndex = 1;

std::vector<ScoreEntry> scoreboard;

std::vector<std::string> mainMenuOptions = {"GRAJ", "USTAWIENIA", "WYJSCIE"};

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

std::vector<std::string> gameSettingsOptions = {
    "Rozpocznij",
    "Szybkosc: Normal",
    "Czestotliwosc: Normal",
    "Rozmiar: 30",
    "Czas: 30s",
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

std::vector<int> timeOptions = {10, 20, 30, 40, 50, 60};
int currentTimeIndex = 0;
int gameTime = timeOptions[currentTimeIndex];
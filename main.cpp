#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
enum class MenuState { MAIN_MENU, SETTINGS, GAME_SETTINGS, GAME };

// ==================================================
// GLOBALNE ZMIENNE
// ==================================================

// Słowa do rozgrywki
std::vector<std::string> words;
std::vector<sf::Text> activeWords;
std::string currentInput;
int score = 0;
float wordSpeed;
float spawnChance; // Szansa w %

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
std::string lobbyMusicFiles = "lobby_music.wav";
std::vector<std::string> gameMusicFiles = {
    "assets/sounds/game_music1.wav",
    "assets/sounds/game_music2.wav",
    "assets/sounds/game_music3.wav"
};

int audioVolume = 5; // Głośność w skali 1-10

// Efekty dźwiękowe (menu i countdown)
sf::SoundBuffer menuMoveBuffer;
sf::Sound menuMoveSound;
sf::SoundBuffer menuSelectBuffer;
sf::Sound menuSelectSound;
sf::SoundBuffer countdownBuffer;
sf::Sound countdownSound;

// Rozmiar wyświetlanych słów
int wordSize = 30; // Domyślny rozmiar słów
std::vector<int> wordSizeOptions = {20, 30, 40, 50}; // Dostępne rozmiary
int currentWordSizeIndex = 1; // Domyślny indeks

// ==================================================
// FUNKCJE ŁADOWANIA ZASOBÓW
// ==================================================

// --- Ładowanie słów z pliku
void loadWordsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }
    std::string word;
    while (std::getline(file, word)) {
        if (!word.empty())
            words.push_back(word);
    }
    file.close();
}

// --- Funkcja do spawnowania nowego słowa na ekranie
void spawnWord(const sf::Font& font, const sf::RenderWindow& window) {
    if (!words.empty()) {
        sf::Text word(words[rand() % words.size()], font, wordSize);
        int topLineY = 60; // Górna linia
        int bottomLineY = window.getSize().y - 50; // Dolna linia
        int maxY = window.getSize().y - wordSize - 50; // Z uwzględnieniem dolnej linii
        int randomY;
        // Generowanie pozycji z uwzględnieniem marginesów
        do {
            randomY = rand() % (maxY - topLineY - 1) + topLineY + wordSize;
        } while (std::abs(randomY - bottomLineY) < wordSize);
        word.setPosition(0, randomY);

        // Sprawdzenie kolizji z już istniejącymi słowami
        bool overlaps = false;
        for (const auto& activeWord : activeWords) {
            if (std::abs(activeWord.getPosition().y - word.getPosition().y) < wordSize) {
                overlaps = true;
                break;
            }
        }
        if (!overlaps) {
            word.setFillColor(sf::Color::White);
            activeWords.push_back(word);
        }
    }
}

// ==================================================
// FUNKCJE AUDIO
// ==================================================

// --- Ładowanie efektów dźwiękowych
void loadAudioEffects() {
    if (!menuMoveBuffer.loadFromFile("assets/sounds/menu_move.wav")) {
        std::cerr << "Nie można załadować pliku: menu_move.wav" << std::endl;
    }
    if (!menuSelectBuffer.loadFromFile("assets/sounds/menu_select.wav")) {
        std::cerr << "Nie można załadować pliku: menu_select.wav" << std::endl;
    }
    if (!countdownBuffer.loadFromFile("assets/sounds/countdown.wav")) {
        std::cerr << "Nie można załadować pliku: countdown.wav" << std::endl;
    }
    menuMoveSound.setBuffer(menuMoveBuffer);
    menuSelectSound.setBuffer(menuSelectBuffer);
    menuMoveSound.setVolume(audioVolume * 10);
    menuSelectSound.setVolume(audioVolume * 10);
    countdownSound.setBuffer(countdownBuffer);
    countdownSound.setVolume(audioVolume * 10);
}

// --- Odtwarzanie losowej muzyki gry
void playRandomGameMusic(int audioVolume) {
    if (!gameMusicFiles.empty()) {
        std::string selectedMusic = gameMusicFiles[rand() % gameMusicFiles.size()];
        if (!gameMusic.openFromFile(selectedMusic)) {
            std::cerr << "Nie można załadować pliku muzycznego: " << selectedMusic << std::endl;
            return;
        }
        gameMusic.setLoop(true);
        gameMusic.setVolume(audioVolume * 10);
        gameMusic.play();
    }
}

// ==================================================
// FUNKCJE GRY
// ==================================================

// --- Funkcja rozgrywki
void runGame(sf::RenderWindow& window, std::string speedSetting, std::string frequencySetting) {
    lobbyMusic.stop();

    // Ustawianie prędkości słów na podstawie ustawień
    if (speedSetting == "Slow") wordSpeed = 70.f;
    else if (speedSetting == "Normal") wordSpeed = 85.f;
    else if (speedSetting == "Fast") wordSpeed = 135.f;
    else if (speedSetting == "ULTRA FAST") wordSpeed = 150.f;
    else if (speedSetting == "MONKEY SPEED!!!") wordSpeed = 173.f;

    // Ustawianie częstotliwości spawnowania słów
    if (frequencySetting == "Low") spawnChance = 1.0f;
    else if (frequencySetting == "Normal") spawnChance = 2.1f;
    else if (frequencySetting == "High") spawnChance = 3.1f;
    else if (frequencySetting == "MONKEY TYPE-LIKE!!!") spawnChance = 5.5f;

    // Odliczanie przed rozpoczęciem gry
    for (int i = 3; i > 0; --i) {
        if (countdownSound.getStatus() != sf::Sound::Playing)
            countdownSound.play();
        window.clear();
        sf::Text countdownText(std::to_string(i), font, 100);
        if (i == 3) countdownText.setFillColor(sf::Color::Green);
        else if (i == 2) countdownText.setFillColor(sf::Color::Yellow);
        else if (i == 1) countdownText.setFillColor(sf::Color::Red);
        countdownText.setPosition(window.getSize().x / 2 - 30, window.getSize().y / 2 - 50);
        window.draw(countdownText);
        window.display();
        sf::sleep(sf::seconds(1.0));
    }

    playRandomGameMusic(audioVolume);
    gameClock.restart();
    sf::Clock gameTimer;
    const int gameDuration = 60; // Czas gry w sekundach

    while (true) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameMusic.stop();
                lobbyMusic.play();
                return;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !currentInput.empty())
                    currentInput.pop_back();
                else if (event.text.unicode == '\r') {
                    for (auto it = activeWords.begin(); it != activeWords.end(); ++it) {
                        if (it->getString() == currentInput) {
                            activeWords.erase(it);
                            score += 100;
                            break;
                        }
                    }
                    currentInput = "";
                }
                else if (event.text.unicode < 128)
                    currentInput += static_cast<char>(event.text.unicode);
            }
        }

        float deltaTime = gameClock.restart().asSeconds();
        for (auto it = activeWords.begin(); it != activeWords.end();) {
            it->move(wordSpeed * deltaTime, 0);
            if (it->getPosition().x > window.getSize().x) {
                it = activeWords.erase(it);
                score -= 175;
            } else {
                ++it;
            }
        }

        if (rand() % 100 < spawnChance)
            spawnWord(font, window);

        int timeLeft = gameDuration - (gameTimer.getElapsedTime().asSeconds());
        if (timeLeft <= 0) {
            gameMusic.stop();
            lobbyMusic.play();
            return;
        }

        // Rysowanie elementów gry
        window.clear();
        sf::RectangleShape headerLine(sf::Vector2f(window.getSize().x, 2));
        headerLine.setPosition(0, 78);
        headerLine.setFillColor(sf::Color::White);
        window.draw(headerLine);

        // Input box
        float inputBoxWidth = window.getSize().x * 0.3f;
        float inputBoxHeight = inputBoxWidth / 10;
        float inputBoxX = window.getSize().x - inputBoxWidth - 20;
        float inputBoxY = 40;
        sf::RectangleShape inputBox(sf::Vector2f(inputBoxWidth, inputBoxHeight));
        inputBox.setPosition(inputBoxX, inputBoxY);
        inputBox.setFillColor(sf::Color(60, 76, 88));
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(sf::Color::White);
        sf::Text inputText(currentInput, font, 20);
        inputText.setFillColor(sf::Color::Green);
        inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + (inputBoxHeight - inputText.getCharacterSize()) / 2);
        window.draw(inputBox);
        window.draw(inputText);

        // Nagłówki
        sf::Text settingsText("Szybkosc: " + speedSetting, font, 20);
        settingsText.setPosition(10, 10);
        settingsText.setFillColor(sf::Color::White);
        window.draw(settingsText);

        sf::Text frequencyText("Czestotliwosc: " + frequencySetting, font, 20);
        frequencyText.setPosition(10, 40);
        frequencyText.setFillColor(sf::Color::White);
        window.draw(frequencyText);

        sf::Text timeText("Czas: " + std::to_string(timeLeft) + "s", font, 20);
        timeText.setPosition(window.getSize().x / 2 - 50, 10);
        timeText.setFillColor(sf::Color::White);
        window.draw(timeText);

        sf::Text scoreText("Punkty: " + std::to_string(score), font, 20);
        scoreText.setPosition(window.getSize().x - 150, 10);
        scoreText.setFillColor(sf::Color::White);
        window.draw(scoreText);

        // Rysowanie słów
        for (const auto& word : activeWords)
            window.draw(word);

        window.display();
    }
}

// ==================================================
// FUNKCJA GŁÓWNA
// ==================================================
int main() {
    // Inicjalizacja okna
    sf::RenderWindow window(sf::VideoMode(900, 660), "Monkey Typer");
    window.setFramerateLimit(60);

    // Ładowanie ikony
    sf::Image icon;
    if (!icon.loadFromFile("assets/images/icon.png")) {
        std::cerr << "Nie można załadować pliku ikony!" << std::endl;
        return -1;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Ładowanie czcionki
    if (!font.loadFromFile("assets/fonts/BrokenConsole.ttf")) {
        std::cerr << "Brak czcionki!" << std::endl;
        return -1;
    }

    // Ładowanie słów
    loadWordsFromFile("Words.txt");
    if (words.empty()) {
        std::cerr << "Brak słów w pliku!" << std::endl;
        return -1;
    }

    // Ładowanie muzyki lobby
    if (!lobbyMusic.openFromFile("assets/sounds/lobby_music.wav")) {
        std::cerr << "Nie można załadować pliku muzycznego!" << std::endl;
        return -1;
    }
    lobbyMusic.setLoop(true);
    lobbyMusic.setVolume(audioVolume * 10);
    lobbyMusic.play();

    // Ładowanie efektów dźwiękowych
    loadAudioEffects();

    srand(static_cast<int>(time(nullptr)));

    // Inicjalizacja stanu menu
    MenuState currentMenu = MenuState::MAIN_MENU;

    // Tekst tytułowy
    sf::Text title("Monkey Typer", font, 60);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Cyan);

    // ==================================================
    // MENU GŁÓWNE
    // ==================================================
    std::vector<std::string> mainMenuOptions = { "GRAJ", "USTAWIENIA", "WYJSCIE" };
    int selectedMainOption = 0;
    std::vector<sf::Text> mainMenuTexts;
    for (size_t i = 0; i < mainMenuOptions.size(); ++i) {
        sf::Text text(mainMenuOptions[i], font, 40);
        text.setPosition(300, 200 + i * 60);
        text.setFillColor(i == selectedMainOption ? sf::Color::Yellow : sf::Color::White);
        mainMenuTexts.push_back(text);
    }

    // ==================================================
    // MENU USTAWIEŃ
    // ==================================================
    std::vector<std::string> settingsOptions = { "Rozdzielczosc: 900x660", "FPS: 60", "Czcionka: ", "Glosnosc: 5/10", "Powrot" };
    int selectedSettingsOption = 0;
    std::vector<sf::Vector2u> resolutions = { {900,660}, {1024,768}, {1280,720}, {1300,1024} };
    std::vector<int> fpsOptions = { 30, 60, 120 };
    int currentResolutionIndex = 0;
    int currentFpsIndex = 1;

    // ==================================================
    // MENU USTAWIEŃ GRY
    // ==================================================
    std::vector<std::string> gameSettingsOptions = {
        "Rozpocznij",
        "Szybkosc slow: Normal",
        "Czestotliwosc: Normal",
        "Rozmiar slow: 30",
        "Preset: Medium",
        "Powrot"
    };
    int selectedGameSettingsOption = 0;
    std::vector<std::string> speedOptions = { "Slow", "Normal", "Fast", "ULTRA FAST", "MONKEY SPEED!!!" };
    std::vector<std::string> frequencyOptions = { "Low", "Normal", "High", "MONKEY TYPE-LIKE!!!" };
    std::vector<std::string> presets = { "Easy", "Medium", "Hard", "mmOnkEy" };
    int currentSpeedIndex = 1;
    int currentFrequencyIndex = 1;
    int currentPresetIndex = 1;
    std::string gameSpeed = "Normal";
    std::string gameFrequency = "Normal";

    // ==================================================
    // GŁÓWNA PĘTLA PROGRAMU
    // ==================================================
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Obsługa zamknięcia okna
            if (event.type == sf::Event::Closed)
                window.close();

            // ==================================================
            // OBSŁUGA MENU GŁÓWNEGO
            // ==================================================
            if (currentMenu == MenuState::MAIN_MENU) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedMainOption = (selectedMainOption - 1 + mainMenuOptions.size()) % mainMenuOptions.size();
                        menuMoveSound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        selectedMainOption = (selectedMainOption + 1) % mainMenuOptions.size();
                        menuMoveSound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        menuSelectSound.play();
                        if (selectedMainOption == 0)
                            currentMenu = MenuState::GAME_SETTINGS;
                        else if (selectedMainOption == 1)
                            currentMenu = MenuState::SETTINGS;
                        else if (selectedMainOption == 2)
                            window.close();
                    }
                    // Aktualizacja kolorów opcji menu
                    for (size_t i = 0; i < mainMenuTexts.size(); ++i)
                        mainMenuTexts[i].setFillColor(i == selectedMainOption ? sf::Color::Yellow : sf::Color::White);
                }
            }
            // ==================================================
            // OBSŁUGA MENU USTAWIEŃ
            // ==================================================
            else if (currentMenu == MenuState::SETTINGS) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedSettingsOption = (selectedSettingsOption - 1 + settingsOptions.size()) % settingsOptions.size();
                        menuMoveSound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        selectedSettingsOption = (selectedSettingsOption + 1) % settingsOptions.size();
                        menuMoveSound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        menuSelectSound.play();
                        if (selectedSettingsOption == 0) {
                            currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();
                            settingsOptions[0] = "Rozdzielczosc: " + std::to_string(resolutions[currentResolutionIndex].x) + "x" +
                                std::to_string(resolutions[currentResolutionIndex].y);
                            window.create(sf::VideoMode(resolutions[currentResolutionIndex].x, resolutions[currentResolutionIndex].y), "Monkey Typer");
                        }
                        else if (selectedSettingsOption == 1) {
                            currentFpsIndex = (currentFpsIndex + 1) % fpsOptions.size();
                            settingsOptions[1] = "FPS: " + std::to_string(fpsOptions[currentFpsIndex]);
                            window.setFramerateLimit(fpsOptions[currentFpsIndex]);
                        }
                        else if (selectedSettingsOption == 2) {
                            currentFontIndex = (currentFontIndex + 1) % fontOptions.size();
                            settingsOptions[2] = "Czcionka: " + fontOptions[currentFontIndex];
                            if (!font.loadFromFile(fontOptions[currentFontIndex]))
                                std::cerr << "Nie można załadować czcionki: " << fontOptions[currentFontIndex] << std::endl;
                            else
                                settingsOptions[2] = "Czcionka: " + fontNames[currentFontIndex];
                        }
                        else if (selectedSettingsOption == 3) {
                            audioVolume = (audioVolume % 10) + 1;
                            settingsOptions[3] = "Glosnosc: " + std::to_string(audioVolume) + "/10";
                            lobbyMusic.setVolume(audioVolume * 10);
                            menuMoveSound.setVolume(audioVolume * 10);
                            menuSelectSound.setVolume(audioVolume * 10);
                        }
                        else if (selectedSettingsOption == 4) {
                            currentMenu = MenuState::MAIN_MENU;
                        }
                    }
                }
            }
            // ==================================================
            // OBSŁUGA MENU USTAWIEŃ GRY
            // ==================================================
            else if (currentMenu == MenuState::GAME_SETTINGS) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedGameSettingsOption = (selectedGameSettingsOption - 1 + gameSettingsOptions.size()) % gameSettingsOptions.size();
                        menuMoveSound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        selectedGameSettingsOption = (selectedGameSettingsOption + 1) % gameSettingsOptions.size();
                        menuMoveSound.play();
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        menuSelectSound.play();
                        if (selectedGameSettingsOption == 0) { // Rozpocznij grę
                            gameSpeed = speedOptions[currentSpeedIndex];
                            gameFrequency = frequencyOptions[currentFrequencyIndex];
                            runGame(window, gameSpeed, gameFrequency);
                            currentMenu = MenuState::MAIN_MENU;
                        }
                        else if (selectedGameSettingsOption == 1) { // Zmiana szybkości słów
                            currentSpeedIndex = (currentSpeedIndex + 1) % speedOptions.size();
                            gameSettingsOptions[1] = "Szybkosc slow: " + speedOptions[currentSpeedIndex];
                            gameSettingsOptions[4] = "Preset: Custom";
                        }
                        else if (selectedGameSettingsOption == 2) { // Zmiana częstotliwości
                            currentFrequencyIndex = (currentFrequencyIndex + 1) % frequencyOptions.size();
                            gameSettingsOptions[2] = "Czestotliwosc: " + frequencyOptions[currentFrequencyIndex];
                            gameSettingsOptions[4] = "Preset: Custom";
                        }
                        else if (selectedGameSettingsOption == 3) { // Zmiana rozmiaru słów
                            currentWordSizeIndex = (currentWordSizeIndex + 1) % wordSizeOptions.size();
                            wordSize = wordSizeOptions[currentWordSizeIndex];
                            gameSettingsOptions[3] = "Rozmiar slow: " + std::to_string(wordSize);
                            gameSettingsOptions[4] = "Preset: Custom";
                        }
                        else if (selectedGameSettingsOption == 4) { // Wybór presetu
                            currentPresetIndex = (currentPresetIndex + 1) % presets.size();
                            gameSettingsOptions[4] = "Preset: " + presets[currentPresetIndex];
                            if (currentPresetIndex == 0) { // Easy
                                currentSpeedIndex = 0;
                                currentFrequencyIndex = 0;
                                currentWordSizeIndex = 3;
                            }
                            else if (currentPresetIndex == 1) { // Medium
                                currentSpeedIndex = 1;
                                currentFrequencyIndex = 1;
                                currentWordSizeIndex = 2;
                            }
                            else if (currentPresetIndex == 2) { // Hard
                                currentSpeedIndex = 2;
                                currentFrequencyIndex = 2;
                                currentWordSizeIndex = 1;
                            }
                            else if (currentPresetIndex == 3) { // mmOnkEy
                                currentSpeedIndex = 4;
                                currentFrequencyIndex = 3;
                                currentWordSizeIndex = 0;
                            }
                            wordSize = wordSizeOptions[currentWordSizeIndex];
                            gameSettingsOptions[1] = "Szybkosc slow: " + speedOptions[currentSpeedIndex];
                            gameSettingsOptions[2] = "Czestotliwosc: " + frequencyOptions[currentFrequencyIndex];
                            gameSettingsOptions[3] = "Rozmiar slow: " + std::to_string(wordSize);
                        }
                        else if (selectedGameSettingsOption == 5) { // Powrót do menu głównego
                            currentMenu = MenuState::MAIN_MENU;
                        }
                        else if (selectedGameSettingsOption == 6) { // Opcja "Powrót"
                            currentMenu = MenuState::MAIN_MENU;
                        }
                    }
                }
            }
        }

        // ==================================================
        // RYSOWANIE ELEMENTÓW INTERFEJSU
        // ==================================================
        window.clear();
        if (currentMenu == MenuState::MAIN_MENU) {
            window.draw(title);
            for (const auto& text : mainMenuTexts)
                window.draw(text);
        }
        else if (currentMenu == MenuState::SETTINGS) {
            for (size_t i = 0; i < settingsOptions.size(); ++i) {
                sf::Text text(settingsOptions[i], font, 30);
                text.setPosition(200, 200 + i * 50);
                text.setFillColor(i == selectedSettingsOption ? sf::Color::Yellow : sf::Color::White);
                window.draw(text);
            }
        }
        else if (currentMenu == MenuState::GAME_SETTINGS) {
            for (size_t i = 0; i < gameSettingsOptions.size(); ++i) {
                sf::Text text(gameSettingsOptions[i], font, 30);
                text.setPosition(200, 200 + i * 50);
                text.setFillColor(i == selectedGameSettingsOption ? sf::Color::Yellow : sf::Color::White);
                window.draw(text);
            }
        }
        window.display();
    }

    return 0;
}

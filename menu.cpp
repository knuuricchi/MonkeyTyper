#include "menu.hpp"
#include "loaders.hpp"
#include "globals.hpp"
#include <iostream>
#include "game.hpp"

void initializeMainMenu(std::vector<sf::Text>& mainMenuTexts, sf::Font& font, int& selectedMainOption) {
    for (size_t i = 0; i < mainMenuOptions.size(); ++i) {
        sf::Text text(mainMenuOptions[i], font, 40);
        text.setPosition(300.f, 200.f + i * 60.f);
        text.setFillColor(i == selectedMainOption ? sf::Color::Yellow : sf::Color::White);
        mainMenuTexts.push_back(text);
    }
}

void handleMainMenuInput(sf::RenderWindow& window, MenuState& currentMenu, std::vector<sf::Text>& mainMenuTexts, int& selectedMainOption) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedMainOption = (selectedMainOption - 1 + mainMenuOptions.size()) % mainMenuOptions.size();
                menuMoveSound.play();
            } else if (event.key.code == sf::Keyboard::Down) {
                selectedMainOption = (selectedMainOption + 1) % mainMenuOptions.size();
                menuMoveSound.play();
            } else if (event.key.code == sf::Keyboard::Enter) {
                menuSelectSound.play();
                if (selectedMainOption == 0)
                    currentMenu = MenuState::GAME_SETTINGS;
                else if (selectedMainOption == 1)
                    currentMenu = MenuState::SETTINGS;
                else if (selectedMainOption == 2)
                    window.close();
            }
            for (size_t i = 0; i < mainMenuTexts.size(); ++i)
                mainMenuTexts[i].setFillColor(i == selectedMainOption ? sf::Color::Yellow : sf::Color::White);
        }
    }
}

void drawMainMenu(sf::RenderWindow& window, const sf::Text& title, const std::vector<sf::Text>& mainMenuTexts) {
    window.clear();
    window.draw(title);
    for (const auto& text : mainMenuTexts)
        window.draw(text);
    window.display();
}

void handleSettingsMenuInput(sf::RenderWindow& window, MenuState& currentMenu, int& selectedSettingsOption) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedSettingsOption = (selectedSettingsOption - 1 + settingsOptions.size()) % settingsOptions.size();
                menuMoveSound.play();
            } else if (event.key.code == sf::Keyboard::Down) {
                selectedSettingsOption = (selectedSettingsOption + 1) % settingsOptions.size();
                menuMoveSound.play();
            } else if (event.key.code == sf::Keyboard::Enter) {
                menuSelectSound.play();
                if (selectedSettingsOption == 0) {
                    currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size();
                    settingsOptions[0] = "Rozdzielczosc: " + std::to_string(resolutions[currentResolutionIndex].x) + "x" +
                        std::to_string(resolutions[currentResolutionIndex].y);
                    window.create(sf::VideoMode{resolutions[currentResolutionIndex].x, resolutions[currentResolutionIndex].y}, "Monkey Typer");
                    loadIcon(window, "assets/images/icon.png");
                } else if (selectedSettingsOption == 1) {
                    currentFpsIndex = (currentFpsIndex + 1) % fpsOptions.size();
                    settingsOptions[1] = "FPS: " + std::to_string(fpsOptions[currentFpsIndex]);
                    window.setFramerateLimit(fpsOptions[currentFpsIndex]);
                } else if (selectedSettingsOption == 2) {
                    currentFontIndex = (currentFontIndex + 1) % fontOptions.size();
                    settingsOptions[2] = "Czcionka: " + fontOptions[currentFontIndex];
                    if (!font.loadFromFile(fontOptions[currentFontIndex]))
                        std::cerr << "Nie można załadować czcionki: " << fontOptions[currentFontIndex] << std::endl;
                    else
                        settingsOptions[2] = "Czcionka: " + fontNames[currentFontIndex];
                } else if (selectedSettingsOption == 3) {
                    audioVolume = (audioVolume % 10) + 1;
                    settingsOptions[3] = "Glosnosc: " + std::to_string(audioVolume) + "/10";
                    lobbyMusic.setVolume(audioVolume * 10);
                    menuMoveSound.setVolume(audioVolume * 10);
                    menuSelectSound.setVolume(audioVolume * 10);
                } else if (selectedSettingsOption == 4) {
                    currentMenu = MenuState::MAIN_MENU;
                }
            }
        }
    }
}

void drawSettingsMenu(sf::RenderWindow& window, const std::vector<std::string>& settingsOptions, int selectedSettingsOption) {
    window.clear();
    for (size_t i = 0; i < settingsOptions.size(); ++i) {
        sf::Text text(settingsOptions[i], font, 30);
        text.setPosition(200.f, 200.f + i * 50.f);
        text.setFillColor(i == selectedSettingsOption ? sf::Color::Yellow : sf::Color::White);
        window.draw(text);
    }
    window.display();
}

void handleGameSettingsMenuInput(sf::RenderWindow& window, MenuState& currentMenu, int& selectedGameSettingsOption) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedGameSettingsOption = (selectedGameSettingsOption - 1 + gameSettingsOptions.size()) % gameSettingsOptions.size();
                menuMoveSound.play();
            } else if (event.key.code == sf::Keyboard::Down) {
                selectedGameSettingsOption = (selectedGameSettingsOption + 1) % gameSettingsOptions.size();
                menuMoveSound.play();
            } else if (event.key.code == sf::Keyboard::Enter) {
                menuSelectSound.play();
                if (selectedGameSettingsOption == 0) {
                    gameSpeed = speedOptions[currentSpeedIndex];
                    gameFrequency = frequencyOptions[currentFrequencyIndex];
                    runGame(window, gameSpeed, gameFrequency, gameTime);
                    currentMenu = MenuState::MAIN_MENU;
                } else if (selectedGameSettingsOption == 1) {
                    currentSpeedIndex = (currentSpeedIndex + 1) % speedOptions.size();
                    gameSettingsOptions[1] = "Szybkosc: " + speedOptions[currentSpeedIndex];
                    gameSettingsOptions[5] = "Preset: Custom";
                } else if (selectedGameSettingsOption == 2) {
                    currentFrequencyIndex = (currentFrequencyIndex + 1) % frequencyOptions.size();
                    gameSettingsOptions[2] = "Czestotliwosc: " + frequencyOptions[currentFrequencyIndex];
                    gameSettingsOptions[5] = "Preset: Custom";
                } else if (selectedGameSettingsOption == 3) {
                    currentWordSizeIndex = (currentWordSizeIndex + 1) % wordSizeOptions.size();
                    wordSize = wordSizeOptions[currentWordSizeIndex];
                    gameSettingsOptions[3] = "Rozmiar: " + std::to_string(wordSize);
                    gameSettingsOptions[5] = "Preset: Custom";
                } else if (selectedGameSettingsOption == 4) {
                    currentTimeIndex = (currentTimeIndex + 1) % timeOptions.size();
                    gameTime = timeOptions[currentTimeIndex];
                    gameSettingsOptions[4] = "Czas: " + std::to_string(gameTime) + "s";
                    gameSettingsOptions[5] = "Preset: Custom";
                } else if (selectedGameSettingsOption == 5) {
                    currentPresetIndex = (currentPresetIndex + 1) % presets.size();
                    gameSettingsOptions[5] = "Preset: " + presets[currentPresetIndex];
                    if (currentPresetIndex == 0) {
                        currentSpeedIndex = 0;
                        currentFrequencyIndex = 0;
                        currentWordSizeIndex = 3;
                        currentTimeIndex = 1;
                    } else if (currentPresetIndex == 1) {
                        currentSpeedIndex = 1;
                        currentFrequencyIndex = 1;
                        currentWordSizeIndex = 2;
                        currentTimeIndex = 2;
                    } else if (currentPresetIndex == 2) {
                        currentSpeedIndex = 2;
                        currentFrequencyIndex = 2;
                        currentWordSizeIndex = 1;
                        currentTimeIndex = 0;
                    } else if (currentPresetIndex == 3) {
                        currentSpeedIndex = 4;
                        currentFrequencyIndex = 3;
                        currentWordSizeIndex = 0;
                        currentTimeIndex = 3;
                    }
                    wordSize = wordSizeOptions[currentWordSizeIndex];
                    gameTime = timeOptions[currentTimeIndex];
                    gameSettingsOptions[1] = "Szybkosc: " + speedOptions[currentSpeedIndex];
                    gameSettingsOptions[2] = "Czestotliwosc: " + frequencyOptions[currentFrequencyIndex];
                    gameSettingsOptions[3] = "Rozmiar: " + std::to_string(wordSize);
                    gameSettingsOptions[4] = "Czas: " + std::to_string(gameTime) + "s";
                } else if (selectedGameSettingsOption == 6) {
                    currentMenu = MenuState::MAIN_MENU;
                }
            }
        }
    }
}

void drawGameSettingsMenu(sf::RenderWindow& window, const std::vector<std::string>& gameSettingsOptions, int selectedGameSettingsOption) {
    window.clear();
    for (size_t i = 0; i < gameSettingsOptions.size(); ++i) {
        sf::Text text(gameSettingsOptions[i], font, 30);
        text.setPosition(200.f, 200.f + i * 50.f);
        text.setFillColor(i == selectedGameSettingsOption ? sf::Color::Yellow : sf::Color::White);
        window.draw(text);
    }
    window.display();
}
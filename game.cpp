#include "game.hpp"
#include "globals.hpp"
#include "loaders.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>

void runGame(sf::RenderWindow& window, std::string speedSetting, std::string frequencySetting, int duration) {
    activeWords.clear();
    currentInput = "";
    bool toReset = false;
    score = 0;
    lobbyMusic.stop();
    bool countdownFinished = false;

    if (speedSetting == "Slow") wordSpeed = 70.f;
    else if (speedSetting == "Normal") wordSpeed = 85.f;
    else if (speedSetting == "Fast") wordSpeed = 135.f;
    else if (speedSetting == "ULTRA FAST") wordSpeed = 150.f;
    else if (speedSetting == "MONKEY SPEED!!!") wordSpeed = 173.f;

    if (frequencySetting == "Low") spawnChance = 1.0f;
    else if (frequencySetting == "Normal") spawnChance = 2.1f;
    else if (frequencySetting == "High") spawnChance = 3.1f;
    else if (frequencySetting == "MONKEY TYPE-LIKE!!!") spawnChance = 5.5f;

    for (int i = 3; i > 0; --i) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1) {
                toReset = true;
                break;
            }
        }
        if (toReset) {
            gameMusic.stop();
            runGame(window, speedSetting, frequencySetting, duration);
            return;
        }
        if (countdownSound.getStatus() != sf::Sound::Playing)
            countdownSound.play();
        window.clear();
        sf::Text countdownText(std::to_string(i), font, 100);
        if (i == 3) countdownText.setFillColor(sf::Color::Green);
        else if (i == 2) countdownText.setFillColor(sf::Color::Yellow);
        else if (i == 1) countdownText.setFillColor(sf::Color::Red);
        countdownText.setPosition(window.getSize().x / 2.f - 30.f, window.getSize().y / 2.f - 50.f);
        window.draw(countdownText);
        window.display();
        sf::sleep(sf::seconds(1.f));
    }
    countdownFinished = true;
    playRandomGameMusic(audioVolume);
    gameClock.restart();
    sf::Clock gameTimer;
    int gameDuration = duration;

    float layerWidth = window.getSize().x / 3.0f;
    float greenZoneEnd = layerWidth;
    float yellowZoneEnd = 2 * layerWidth;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameMusic.stop();
                lobbyMusic.play();
                return;
            }
            if (event.key.code == sf::Keyboard::F1) {
                toReset = true;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !currentInput.empty())
                    currentInput.pop_back();
                else if (event.text.unicode == '\r') {
                    for (auto i = activeWords.begin(); i != activeWords.end(); ++i) {
                        if (i->getString() == currentInput) {
                            int wordLength = static_cast<int>(currentInput.length());
                            int points = 100;
                            if (wordLength > 10) points *= 5;
                            else if (wordLength > 6) points *= 2;
                            score += points;
                            activeWords.erase(i);
                            break;
                        }
                    }
                    currentInput = "";
                } else if (event.text.unicode < 128)
                    currentInput += static_cast<char>(event.text.unicode);
            }
        }
        if (toReset) {
            gameMusic.stop();
            runGame(window, speedSetting, frequencySetting, duration);
            return;
        }

        float deltaTime = gameClock.restart().asSeconds();
        layerWidth = window.getSize().x / 3.0f;
        greenZoneEnd = layerWidth;
        yellowZoneEnd = 2 * layerWidth;

        for (auto i = activeWords.begin(); i != activeWords.end();) {
            i->move(wordSpeed * deltaTime, 0);
            float wordX = i->getPosition().x;
            if (wordX < greenZoneEnd) {
                i->setFillColor(sf::Color::Green);
            } else if (wordX < yellowZoneEnd) {
                i->setFillColor(sf::Color::Yellow);
            } else {
                i->setFillColor(sf::Color::Red);
            }
            if (wordX > window.getSize().x) {
                i = activeWords.erase(i);
                score -= 175;
            } else {
                ++i;
            }
        }

        if (countdownFinished && rand() % 100 < spawnChance)
            spawnWord(font, window);

        int timeLeft = gameDuration - static_cast<int>(gameTimer.getElapsedTime().asSeconds());
        if (timeLeft <= 0) {
            gameMusic.stop();
            lobbyMusic.play();
            showPostGameMenu(window);
            return;
        }

        window.clear();
        sf::RectangleShape headerLine(sf::Vector2f(static_cast<float>(window.getSize().x), 2.f));
        headerLine.setPosition(0, 78.f);
        headerLine.setFillColor(sf::Color::White);
        window.draw(headerLine);

        float inputBoxWidth = window.getSize().x * 0.3f;
        float inputBoxHeight = inputBoxWidth / 10.f;
        float inputBoxX = window.getSize().x - inputBoxWidth - 20.f;
        float inputBoxY = 40.f;
        sf::RectangleShape inputBox(sf::Vector2f(inputBoxWidth, inputBoxHeight));
        inputBox.setPosition(inputBoxX, inputBoxY);
        inputBox.setFillColor(sf::Color(60, 76, 88));
        inputBox.setOutlineThickness(2.f);
        inputBox.setOutlineColor(sf::Color::White);
        sf::Text inputText(currentInput, font, 20);
        inputText.setFillColor(sf::Color::Green);
        inputText.setPosition(inputBox.getPosition().x + 10.f, inputBox.getPosition().y + (inputBoxHeight - inputText.getCharacterSize()) / 2.f);
        window.draw(inputBox);
        window.draw(inputText);

        sf::Text settingsText("Szybkosc: " + speedSetting, font, 20);
        settingsText.setPosition(10.f, 10.f);
        settingsText.setFillColor(sf::Color::White);
        window.draw(settingsText);

        sf::Text frequencyText("Czestotliwosc: " + frequencySetting, font, 20);
        frequencyText.setPosition(10.f, 40.f);
        frequencyText.setFillColor(sf::Color::White);
        window.draw(frequencyText);

        sf::Text timeText("Czas: " + std::to_string(timeLeft) + "s", font, 20);
        timeText.setPosition(window.getSize().x / 2.f - 50.f, 10.f);
        timeText.setFillColor(sf::Color::White);
        window.draw(timeText);

        sf::Text scoreText("Punkty: " + std::to_string(score), font, 20);
        scoreText.setPosition(window.getSize().x - 200.f, 10.f);
        scoreText.setFillColor(sf::Color::White);
        window.draw(scoreText);

        for (const auto& word : activeWords)
            window.draw(word);

        window.display();
    }
}

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

void spawnWord(const sf::Font& font, const sf::RenderWindow& window) {
    if (!words.empty()) {
        sf::Text word(words[rand() % words.size()], font, wordSize);
        int topLineY = 60;
        int bottomLineY = window.getSize().y - 50;
        int maxY = window.getSize().y - wordSize - 50;
        int randomY;
        do {
            randomY = rand() % (maxY - topLineY - 1) + topLineY + wordSize;
        } while (std::abs(randomY - bottomLineY) < wordSize);
        word.setPosition(0, static_cast<float>(randomY));

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


void loadScoreboard(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    scoreboard.clear();
    std::string name;
    int score;
    while (file >> name >> score) {
        scoreboard.push_back({name, score});
    }
    file.close();
}

void filterScoreboard(bool ascending) {
    if (ascending) {
        std::sort(scoreboard.begin(), scoreboard.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score < b.score;
        });
    } else {
        std::sort(scoreboard.begin(), scoreboard.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
            return a.score > b.score;
        });
    }
}


void saveScoreboard(const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;

    const auto& lastEntry = scoreboard.back();
    file << lastEntry.playerName << " " << lastEntry.score << "\n";
    file.close();
}

auto showPostGameMenu(sf::RenderWindow& window) -> void {
    lobbyMusic.stop();
    postGameMusic.play();

    scoreboard.push_back({"Local", score});

    std::sort(scoreboard.begin(), scoreboard.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        return b.score > a.score;
    });

    saveScoreboard("scoreboard.txt");

    sf::Text title("Lokalne wyniki", font, 50);
    title.setPosition(200.f, 50.f);
    title.setFillColor(sf::Color::Cyan);

    std::vector<sf::Text> scoreTexts;
    for (size_t i = 0; i < scoreboard.size(); ++i) {
        sf::Text scoreText(
            std::to_string(i + 1) + ". " + scoreboard[i].playerName + ": " + std::to_string(scoreboard[i].score),
            font, 30
        );
        scoreText.setPosition(200.f, 150.f + i * 40.f);
        scoreText.setFillColor(sf::Color::White);
        scoreTexts.push_back(scoreText);
    }

    int selectedOption = 0;
    const int optionCount = 2;
    bool enterPressed = false;
    bool ascending = false;

    sf::Text optionFilter("Filtruj wyniki", font, 30);
    optionFilter.setPosition(200.f, 500.f);

    sf::Text optionMain("Menu", font, 30);
    optionMain.setPosition(200.f, 550.f);

    while (window.isOpen() && !enterPressed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedOption = (selectedOption + optionCount - 1) % optionCount;
                    menuMoveSound.play();
                }
                if (event.key.code == sf::Keyboard::Down) {
                    selectedOption = (selectedOption + 1) % optionCount;
                    menuMoveSound.play();
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedOption == 0) {
                        ascending = !ascending;
                        filterScoreboard(ascending);
                        scoreTexts.clear();
                        for (size_t i = 0; i < scoreboard.size(); ++i) {
                            sf::Text scoreText(
                                std::to_string(i + 1) + ". " + scoreboard[i].playerName + ": " + std::to_string(scoreboard[i].score),
                                font, 30
                            );
                            scoreText.setPosition(200.f, 150.f + i * 40.f);
                            scoreText.setFillColor(sf::Color::White);
                            scoreTexts.push_back(scoreText);
                        }
                        menuSelectSound.play();
                    } else {
                        enterPressed = true;
                        menuSelectSound.play();
                    }
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        optionFilter.setFillColor(selectedOption == 0 ? sf::Color::Yellow : sf::Color::White);
        optionMain.setFillColor(selectedOption == 1 ? sf::Color::Yellow : sf::Color::White);

        window.clear();
        window.draw(title);
        for (const auto& scoreText : scoreTexts)
            window.draw(scoreText);
        window.draw(optionFilter);
        window.draw(optionMain);
        window.display();
    }

    postGameMusic.stop();
    lobbyMusic.play();
}

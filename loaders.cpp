#include "loaders.hpp"
#include <fstream>
#include <iostream>
#include "globals.hpp"

void loadIcon(sf::RenderWindow& window, const std::string& filename) {
    sf::Image icon;
    if (!icon.loadFromFile(filename)) {
        std::cerr << "Nie można załadować pliku ikony: " << filename << std::endl;
        return;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void loadFont(const std::string& filename) {
    if (!font.loadFromFile(fontOptions[currentFontIndex])) {
        std::cerr << "Nie można załadować czcionki: " << std::endl;
    }
}

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

void loadAudioEffects() {
    if (!menuMoveBuffer.loadFromFile("assets/sounds/menu_move.wav")) {
        std::cerr << "Nie można załadować pliku: menu_move.wav" << std::endl;
        return; // Zakończ, jeśli plik nie został załadowany
    }
    if (!menuSelectBuffer.loadFromFile("assets/sounds/menu_select.wav")) {
        std::cerr << "Nie można załadować pliku: menu_select.wav" << std::endl;
        return;
    }
    if (!countdownBuffer.loadFromFile("assets/sounds/countdown.wav")) {
        std::cerr << "Nie można załadować pliku: countdown.wav" << std::endl;
        return;
    }

    menuMoveSound.setBuffer(menuMoveBuffer);
    menuSelectSound.setBuffer(menuSelectBuffer);
    countdownSound.setBuffer(countdownBuffer);

    menuMoveSound.setVolume(audioVolume * 10);
    menuSelectSound.setVolume(audioVolume * 10);
    countdownSound.setVolume(audioVolume * 10);
}

void loadMusic(sf::Music& music, const std::string& filename, int volume) {
    if (!music.openFromFile(filename)) {
        std::cerr << "Nie można załadować pliku muzycznego: " << filename << std::endl;
        return;
    }
    music.setLoop(true);
    music.setVolume(volume * 10);
}
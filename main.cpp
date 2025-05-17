#include <SFML/Graphics.hpp>
#include "menu.hpp"
#include "globals.hpp"
#include "loaders.hpp"
#include <thread>

int main() {
    sf::RenderWindow window(sf::VideoMode{900u, 660u}, "Monkey Typer");
    window.setFramerateLimit(60);

    loadFont("assets/BrokenConsole.ttf");
    loadIcon(window, "assets/images/icon.png");
    loadWordsFromFile("Words.txt");
    loadAudioEffects();
    loadMusic(lobbyMusic, lobbyMusicFiles, audioVolume);
    loadPostGameMuisc(postGameMusic, postGameMusicFiles, audioVolume);

    showLoadingScreen(window, font);
    std::this_thread::sleep_for(std::chrono::milliseconds(900));

    lobbyMusic.play();

    MenuState currentMenu = MenuState::MAIN_MENU;

    sf::Text title("Monkey Typer", font, 60);
    title.setPosition(200.f, 50.f);
    title.setFillColor(sf::Color::Cyan);

    std::vector<sf::Text> mainMenuTexts;
    int selectedMainOption = 0;
    initializeMainMenu(mainMenuTexts, font, selectedMainOption);

    while (window.isOpen()) {
        if (currentMenu == MenuState::MAIN_MENU) {
            handleMainMenuInput(window, currentMenu, mainMenuTexts, selectedMainOption);
            drawMainMenu(window, title, mainMenuTexts);
        } else if (currentMenu == MenuState::SETTINGS) {
            static int selectedSettingsOption = 0;
            handleSettingsMenuInput(window, currentMenu, selectedSettingsOption);
            drawSettingsMenu(window, settingsOptions, selectedSettingsOption);
        } else if (currentMenu == MenuState::GAME_SETTINGS) {
            static int selectedGameSettingsOption = 0;
            handleGameSettingsMenuInput(window, currentMenu, selectedGameSettingsOption);
            drawGameSettingsMenu(window, gameSettingsOptions, selectedGameSettingsOption);
        }
    }

    return 0;
}
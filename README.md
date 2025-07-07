# MonkeyTyper

**MonkeyTyper** to gra napisana w C++ z użyciem biblioteki **SFML**, której celem jest ćwiczenie szybkiego pisania słów na czas. Gracz wpisuje pojawiające się słowa, rywalizując z zegarem, a wszystko odbywa się w przyjaznym interfejsie graficznym z dźwiękami i opcjami ustawień.

## Funkcje

- Dynamiczna rozgrywka oparta na czasie
- Intuicyjne menu główne
- Ustawienia gry (np. poziom trudności, dźwięk)
- Efekty dźwiękowe
- Modularny i czytelny kod C++
- Okno graficzne z obsługą zdarzeń (SFML)

## Wymagania

- C++20
- SFML 2.5 lub nowsza
- CMake
- Na Windowsie: pliki DLL biblioteki SFML (patrz niżej)

## Jak uruchomić

1. Sklonuj repozytorium:
   ```bash
   git clone https://github.com/<nazwa-uzytkownika>/MonkeyTyper.git

    Wejdź do katalogu projektu:

cd MonkeyTyper

Zbuduj projekt za pomocą CMake:

mkdir build
cd build
cmake ..
make

(Windows) Upewnij się, że pliki .dll z paczki SFML znajdują się w katalogu z plikiem wykonywalnym (build/). Wymagane pliki zależą od tego, które moduły SFML są używane (np. sfml-graphics.dll, sfml-window.dll, sfml-system.dll, sfml-audio.dll).

Można je pobrać ze strony: https://www.sfml-dev.org/download.php

Uruchom grę:

    ./MonkeyTyper

Struktura katalogów

MonkeyTyper/
├── assets/         # Czcionki, obrazy, dźwięki
└── src/            # Źródła gry (C++)

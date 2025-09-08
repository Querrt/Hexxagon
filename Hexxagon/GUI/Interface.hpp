#pragma once
#include <vector>

#include "../Core/Board/Field.hpp"
#include "../Core/TurnAndMoves/Turn.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

enum struct ScreenState {
    Menu,
    Game,
    BestGames,
    LoadGame,
    Settings,
    SaveGame
};

class Interface {
    public:
    static void saveGame(const std::vector<Field>& fields, const Turn& turn, const std::string& fileName);
    static void inputGameName(sf::Event::TextEntered event, std::string& nameStr);
    static void displayGameName (sf::RenderWindow& window, const sf::Font& font, std::string& nameStr);

    static std::vector<Field> loadGame(const std::string& fileName,Turn& currentTurn);

    static void saveIfBestGame(int player1Score, int player2Score, const std::string& filePath);
    static std::vector<std::pair<std::string, std::string>> getBestGames(const std::string& filePath);
    static void displayBestGames(
        const std::vector<std::pair<std::string, std::string>>& games,
        sf::RenderWindow& window,
        const sf::Font& font
        );
};
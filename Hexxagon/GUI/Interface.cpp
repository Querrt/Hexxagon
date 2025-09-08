#include "Interface.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "fmt/printf.h"
#include "SFML/Graphics/Text.hpp"


void Interface::saveGame(const std::vector<Field>& fields, const Turn& turn, const std::string& filename) {
    std::fstream file(filename, std::ios::out);

    for (const auto& field : fields)
        file << field.getQ() << " " << field.getR() << " " << Field::toString(field.getState()) << "\n";


    file << "Turn: " << turn.turnNumber << " " << Field::toString(turn.currentPlayer) << "\n";

    file.close();
    fmt::print("Game saved in: {}\n", filename);
}

// https://www.sfml-dev.org/documentation/3.0.0/structsf_1_1Event_1_1TextEntered.html
void Interface::inputGameName(sf::Event::TextEntered event, std::string& nameStr) {
    const auto chr = static_cast<char>(event.unicode);

    //backspace
    if (event.unicode == 8) {
        if (!nameStr.empty()) {
            nameStr.pop_back();
        }
    } else
        nameStr += chr;
}

void Interface::displayGameName(sf::RenderWindow &window, const sf::Font &font, std::string &nameStr) {
    sf::Text infoText(font, "Input game name:");
    infoText.setCharacterSize(40);
    infoText.setFillColor(sf::Color::White);
    infoText.setOutlineColor(sf::Color::Black);
    infoText.setOutlineThickness(2);
    infoText.setOrigin({0, infoText.getGlobalBounds().size.y / 2.f});
    infoText.setPosition({20, window.getSize().y/2.f - 40});

    sf::Text nameText(font, nameStr);
    nameText.setCharacterSize(20);
    nameText.setFillColor(sf::Color::White);
    nameText.setOutlineColor(sf::Color::Black);
    nameText.setOutlineThickness(2);
    nameText.setOrigin({0, nameText.getGlobalBounds().size.y / 2.f});
    nameText.setPosition({20, window.getSize().y/2.f});

    window.draw(infoText);
    window.draw(nameText);
}


std::vector<Field> Interface::loadGame(const std::string& fileName, Turn& currentTurn) {
    std::ifstream file(fileName);

    std::vector<Field> fields;
    std::string line;

    fmt::println("1");

    while (std::getline(file, line)) {
        if (line.find("Turn:") != std::string::npos) {
            try {
                std::istringstream inStream(line);
                std::string turnStr;
                int turnNumber;
                std::string playerStr;
                FieldState currentPlayer;

                inStream >> turnStr >> turnNumber >> playerStr;
                currentTurn.turnNumber = turnNumber;

                if (playerStr == "Player1")
                    currentPlayer = FieldState::Player1;
                else
                    currentPlayer = FieldState::Player2;

                currentTurn.currentPlayer = currentPlayer;
            }
            catch (const std::invalid_argument& e) {
                fmt::print("Invalid input: {}\n", e.what());
            }
        }
        else {
            auto it1 = std::ranges::find(line, ' ');
            auto it2 = (it1 != line.end()) ? std::ranges::find(it1 + 1, line.end(), ' ') : line.end();

            if (it1 != line.end() && it2 != line.end()) {
                std::string qStr(line.begin(), it1);
                std::string rStr(it1 + 1, it2);
                std::string stateStr(it2 + 1, line.end());

                int q = std::stoi(qStr);
                int r = std::stoi(rStr);

                FieldState state;
                bool validState =
                    (stateStr == "None" && (state = FieldState::None, true)) ||
                    (stateStr == "Player1" && (state = FieldState::Player1, true)) ||
                    (stateStr == "Player2" && (state = FieldState::Player2, true)) ||
                    (stateStr == "NotAvailable" && (state = FieldState::NotAvailable, true));

                if (validState) {
                    fields.emplace_back(q, r, state);
                }
            }
        }
    }
    file.close();
    return fields;
}

void Interface::saveIfBestGame(int player1Score, int player2Score, const std::string& filePath) {
    const std::string filename = filePath;
    std::vector<std::pair<double, std::string>> bestGames;

    // wczytujemy istniejace gierki
    std::ifstream inFile(filename);
    std::string line;
    while (std::getline(inFile, line)) {
        auto spaceIndex = line.find(' ');
        if (spaceIndex != std::string::npos) {
            // stod to jak stoi, tylko dla dabli babli
            double ratio = std::stod(line.substr(0, spaceIndex));
            auto date = line.substr(spaceIndex + 1);
            bestGames.emplace_back(ratio, date);
        }
    }
    inFile.close();

    double scoreRatio = player2Score == 0 ?
        std::numeric_limits<double>::infinity() :
        static_cast<double>(player1Score) / player2Score;

    // pobieranie daty: https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
    auto now = std::chrono::system_clock::now();
    std::string dateStr = std::format("{:%Y-%m-%d %H:%M}", now);

    bestGames.emplace_back(scoreRatio, dateStr);

    std::sort(bestGames.begin(), bestGames.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    // zapisujemy tylko 3 najlepsze gry
    if (bestGames.size() > 3) {
        bestGames.erase(bestGames.begin() + 3, bestGames.end());
    }

    // nadpisujemy poprzendi stan pliku
    std::ofstream outFile(filename);
    for (const auto& [sr, date] : bestGames) {
        outFile << std::fixed << std::setprecision(2) << sr << " " << date << "\n";
    }
    outFile.close();
}


std::vector<std::pair<std::string, std::string>> Interface::getBestGames(const std::string& filePath) {
    const std::string filename = filePath;
    std::ifstream inFile(filename);

    std::vector<std::pair<std::string, std::string>> bestGames;

    std::string line;
    while (std::getline(inFile, line)) {
        auto spaceIndex = line.find(' ');
        if (spaceIndex != std::string::npos) {
            std::string ratio = line.substr(0, spaceIndex);
            std::string date = line.substr(spaceIndex + 1);
            bestGames.emplace_back(ratio, date);
        }
    }
    inFile.close();

    return bestGames;
}

void Interface::displayBestGames(
    const std::vector<std::pair<std::string, std::string>>& games,
    sf::RenderWindow& window,
    const sf::Font& font
    ) {
    sf::Text bestGamesText(font, "BEST GAMES");
    bestGamesText.setCharacterSize(60);
    bestGamesText.setFillColor(sf::Color::Yellow);
    bestGamesText.setOutlineColor(sf::Color::Black);
    bestGamesText.setOutlineThickness(2);
    bestGamesText.setPosition(
        {window.getSize().x/2 - bestGamesText.getGlobalBounds().size.x/2,
            20}
            );

    sf::Text scoreText(font, "Score:");
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition({100, 120});

    sf::Text dateText(font, "Played on:");
    dateText.setCharacterSize(40);
    dateText.setFillColor(sf::Color::White);
    dateText.setOutlineColor(sf::Color::Black);
    dateText.setOutlineThickness(2);
    dateText.setPosition({300, 120});


    for (int i = 0; i < games.size(); i++) {
        sf::Text gameScore(font, games[i].first);
        gameScore.setCharacterSize(24);
        gameScore.setFillColor(sf::Color::White);
        gameScore.setOutlineColor(sf::Color::Black);
        gameScore.setOutlineThickness(2);
        gameScore.setPosition({100, 180 + i * 40.f});

        sf::Text gameDate(font, games[i].second);
        gameDate.setCharacterSize(24);
        gameDate.setFillColor(sf::Color::White);
        gameDate.setOutlineColor(sf::Color::Black);
        gameDate.setOutlineThickness(2);
        gameDate.setPosition({300, 180 + i * 40.f});

        window.draw(gameScore);
        window.draw(gameDate);
    }

    window.draw(scoreText);
    window.draw(dateText);
    window.draw(bestGamesText);
}

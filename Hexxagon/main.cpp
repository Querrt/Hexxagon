#include <iostream>
#include <thread>

#include "Core/Board/Board.hpp"
#include "Core/Board/BoardCreator.hpp"
#include "Core/TurnAndMoves/Ai.hpp"
#include "Core/TurnAndMoves/Game.hpp"
#include "Core/TurnAndMoves/Move.hpp"
#include "Core/TurnAndMoves/Turn.hpp"
#include "fmt/color.h"
#include "GUI/Button.hpp"
#include "GUI/Interface.hpp"
#include "GUI/Score.hpp"


static std::string toString(FieldState state) {
    switch(state) {
        case FieldState::None: return "None";
        case FieldState::Player1: return "Player1";
        case FieldState::Player2: return "Player2";
        case FieldState::NotAvailable: return "NotAvailable";
        default: return "Unknown";
    }
}

int main() {

    static const sf::Font font(R"(assets/airstrike.ttf)");
    static const auto bestGamesFilePath = R"(assets/bestGames.txt)";
    static const auto savedGamesFilePath = R"(assets/saves/)";
    auto isOpponentAi = true;
    std::string saveGameName;

    sf::RenderWindow window(
        sf::VideoMode({800, 600}), "Hexxagon",
        sf::Style::Default, sf::State::Windowed, {.antiAliasingLevel = 8}
        );
    ScreenState screenState = ScreenState::Menu;
    // tło z filmu: https://www.youtube.com/watch?v=bOG8667yePY
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(800, 600));
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile(R"(assets/Background.png)");
    background.setTexture(&backgroundTexture);

    Button playButton({140, 70}, font, "Play");
    playButton.setFontSize(50);
    playButton.setPosition({window.getSize().x/2.f, window.getSize().y/2.f});

    Button settingsButton({120, 40}, font, "Settings");
    settingsButton.setPosition({window.getSize().x - 60.f, 20});

    Button bestGamesButton({160, 40}, font, "Best Games");
    bestGamesButton.setPosition({window.getSize().x - 90.f, window.getSize().y - 60.f});

    Button loadGameButton({140, 40}, font, "Load Game");
    loadGameButton.setPosition({window.getSize().x - 90.f, window.getSize().y - 20.f});

    Button saveGameButton({120, 40}, font, "Save Game");
    saveGameButton.setPosition({80, 30});

    Button menuButton({100, 40}, font, "Back");
    menuButton.setPosition({50, 20});

    Button chooseOpponentButton({100, 60}, font, "PvE");
    chooseOpponentButton.setPosition({window.getSize().x/2.f, window.getSize().y/2.f});
    chooseOpponentButton.setFontSize(40);

    Button endGameButton({800, 600}, font, "");
    endGameButton.setPosition({400,300});

    Button returnToGameButton({200, 40}, font, "Return to Game");
    returnToGameButton.setPosition({window.getSize().x - 120.f, 20});

    Button saveAndEndGameButton({100, 40}, font, "Save");
    saveAndEndGameButton.setPosition({50, 20});

    Button exitWithoutSavingButton({290, 40}, font, "Exit without saving");
    exitWithoutSavingButton.setPosition({145, 60});

    auto fields = createBoard();
    Board board(fields);
    Turn turn;
    Score score;
    auto changePlayer = bool();
    auto static savedInBest = false;

    window.setFramerateLimit(60);

    auto border = new sf::RectangleShape(sf::Vector2f(1, window.getSize().y));
    border->setFillColor(sf::Color::Black);
    border->setPosition(sf::Vector2f(window.getSize().x - 30, 0));

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if(event->is<sf::Event::Closed>())
                window.close();

                // Menu -------------------------------------------------------------------
            if (screenState == ScreenState::Menu) {
                if (playButton.isClicked(window, event))
                    screenState = ScreenState::Game;
                if (settingsButton.isClicked(window, event)) {
                    screenState = ScreenState::Settings;
                }
                if (bestGamesButton.isClicked(window, event)) {
                    screenState = ScreenState::BestGames;
                }
                if (loadGameButton.isClicked(window, event)) {
                    screenState = ScreenState::LoadGame;
                }
                // Game -------------------------------------------------------------------
            } else if (screenState == ScreenState::Game) {
                if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    changePlayer = board.click(window, mousePos, turn);
                }
                if (score.winner == GameState::InGame && saveGameButton.isClicked(window, event)) {
                    screenState = ScreenState::SaveGame;
                }
                if (endGameButton.isClicked(window, event) && score.winner != GameState::InGame) {
                    screenState = ScreenState::Menu;
                    board.resetBoard();
                    score.winner = GameState::InGame;
                    savedInBest = false;
                    turn.turnNumber = 1;
                    turn.currentPlayer = FieldState::Player1;
                }
                // Settings --------------------------------------------------------------
            } else if (screenState == ScreenState::Settings) {
                if (menuButton.isClicked(window, event))
                    screenState = ScreenState::Menu;
                if (chooseOpponentButton.isClicked(window, event)) {
                    if (isOpponentAi) {
                        chooseOpponentButton.setText("PvP");
                        isOpponentAi = false;
                    } else {
                        chooseOpponentButton.setText("PvE");
                        isOpponentAi = true;
                    }
                }
                // BestGames--------------------------------------------------------------
            } else if (screenState == ScreenState::BestGames) {
                if (menuButton.isClicked(window, event))
                    screenState = ScreenState::Menu;
                // LoadGame ---------------------------------------------------------------
            } else if (screenState == ScreenState::LoadGame) {
                if (menuButton.isClicked(window, event)) {
                    screenState = ScreenState::Menu;
                    saveGameName = "";
                }
                if (auto textEntered = event->getIf<sf::Event::TextEntered>()) {
                    Interface::inputGameName(*textEntered, saveGameName);
                }
                if (loadGameButton.isClicked(window, event)) {
                    std::string gameFullPath = savedGamesFilePath;
                    gameFullPath += "\\" + saveGameName + ".txt";
                    fields = Interface::loadGame(gameFullPath, turn);
                    saveGameName = "";
                    screenState = ScreenState::Game;
                }
                // SaveGame ---------------------------------------------------------------
            } else if (screenState == ScreenState::SaveGame) {
                if (returnToGameButton.isClicked(window, event)) {
                    screenState = ScreenState::Game;
                    saveGameName = "";
                }
                if (saveAndEndGameButton.isClicked(window, event)) {
                    screenState = ScreenState::Menu;
                    std::string saveFullPath = savedGamesFilePath;
                    saveFullPath += "\\" + saveGameName + ".txt";

                    Interface::saveGame(fields, turn, saveFullPath);

                    saveGameName = "";
                }
                if (auto textEntered = event->getIf<sf::Event::TextEntered>()) {
                    Interface::inputGameName(*textEntered, saveGameName);
                }
                if (exitWithoutSavingButton.isClicked(window, event)) {
                    screenState = ScreenState::Menu;
                    saveGameName = "";
                    fields = createBoard();
                }
            }
        }

        window.clear();
        window.draw(background);

        // Menu --------------------------------------------------------------
        if (screenState == ScreenState::Menu) {
            playButton.mouseHover(window);
            playButton.draw(window);
            settingsButton.mouseHover(window);
            settingsButton.draw(window);
            bestGamesButton.mouseHover(window);
            bestGamesButton.draw(window);
            loadGameButton.mouseHover(window);
            loadGameButton.draw(window);
        }

        // Settings ----------------------------------------------------------
        else if (screenState == ScreenState::Settings) {
            menuButton.mouseHover(window);
            menuButton.draw(window);
            chooseOpponentButton.mouseHover(window);
            chooseOpponentButton.draw(window);


        }
        // Best Games --------------------------------------------------------
        else if (screenState == ScreenState::BestGames) {
            menuButton.mouseHover(window);
            menuButton.draw(window);

            auto bestGamesData = Interface::getBestGames(bestGamesFilePath);
            Interface::displayBestGames(bestGamesData, window, font);
        }
        // Load Game ---------------------------------------------------------
        else if (screenState == ScreenState::LoadGame) {

            menuButton.mouseHover(window);
            menuButton.draw(window);
            loadGameButton.mouseHover(window);
            loadGameButton.draw(window);

            Interface::displayGameName(window, font, saveGameName);
        }
        // SaveGame ----------------------------------------------------------
        else if (screenState == ScreenState::SaveGame) {
            saveAndEndGameButton.mouseHover(window);
            saveAndEndGameButton.draw(window);
            returnToGameButton.mouseHover(window);
            returnToGameButton.draw(window);
            exitWithoutSavingButton.mouseHover(window);
            exitWithoutSavingButton.draw(window);

            Interface::displayGameName(window, font, saveGameName);
        }
        // Game --------------------------------------------------------------
        else if (screenState == ScreenState::Game) {
            if (score.winner == GameState::InGame)
                saveGameButton.mouseHover(window);
            saveGameButton.draw(window);

            if (isOpponentAi && score.winner == GameState::InGame) {
                if (Ai::isAiTurn(turn)) {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    auto [from, to] = Ai::chooseBestMove(fields, turn);
                    Ai::commitMove(from, to, fields, turn);
                    changePlayer = true;
                    fmt::print("Bot");
                }
            }

            if (
                (!Game::hasViableMove(turn, fields) || !Game::hasPawn(score)) &&
                score.winner == GameState::InGame &&
                turn.turnNumber != 1
                ) {
                score.winner = Game::determineWinner(score);
                if (isOpponentAi && !savedInBest) {
                    Interface::saveIfBestGame(score.player1Score, score.player2Score, bestGamesFilePath);
                    savedInBest = true;
                }
            }

            if (changePlayer) {
                if (turn.currentPlayer == FieldState::Player1)
                    turn.currentPlayer = FieldState::Player2;
                else {
                    turn.currentPlayer = FieldState::Player1;
                    turn.turnNumber++;
                }
                changePlayer = false;
            }
            score.countPoints(fields);

            sf::Text illegalMoveText(font, "");
            illegalMoveText.setCharacterSize(30);
            illegalMoveText.setFillColor(sf::Color::Red);
            illegalMoveText.setOutlineColor(sf::Color::Black);
            illegalMoveText.setOutlineThickness(2);

            // if (Board::displayIllegalMoveTextTooLongDistance) {
            //     illegalMoveText.setString("Too long distance");
            //     Board::displayIllegalMoveText = true;
            //     Board::displayIllegalMoveTextTooLongDistance = false;
            // }
            // if (Board::displayIllegalMoveTextFieldTaken) {
            //     illegalMoveText.setString("Field taken");
            //     Board::displayIllegalMoveText = true;
            //     Board::displayIllegalMoveTextFieldTaken = false;
            // }
            // if (Board::displayIllegalMoveTextNotAField) {
            //     illegalMoveText.setString("It is not a field");
            //     Board::displayIllegalMoveText = true;
            //     Board::displayIllegalMoveTextNotAField = false;
            // }
            //
            // if (Board::displayIllegalMoveText) {
            //     illegalMoveText.setOrigin(illegalMoveText.getGlobalBounds().size/2.f);
            //     illegalMoveText.setPosition({window.getSize().x/2.f, 100});
            //
            //     window.draw(illegalMoveText);
            //     std::this_thread::sleep_for(std::chrono::seconds(1));
            //     illegalMoveText.setCharacterSize(0);
            //     Board::displayIllegalMoveText = false;
            // }


            // rysowanie planszy
            board.draw(window);
            score.drawPlayerTurnText(window, font, turn);
            score.drawScoreBoard(window, font, turn);

            if (score.winner != GameState::InGame)
                Game::endGame(score, font, window);

        }
        window.display();
    }
    return 0;
}

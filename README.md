# Hexxagon
**Hexxagon** is a turn-based strategy board game implemented in C++.  
The game is played on a hexagonal grid where two players compete to occupy as many tiles as possible.

## Technologies
- C++23
- SFML (GUI)
- FMT
- CLion

## Features
- **PvE** - play against custom AI algorithm.
- **PvP** - play with a friend.
- Turn-based mechanics.
- Score tracking and visual feedback.
- Highlighted possible moves for chosen piece.
- Save and load a game from a file.
- Best game scores can be viewed from the menu.

## How to run
1. Clone this repository.
2. Open the project folder in CLion (or any IDE with CMake support).
3. Make sure SFML and FMT are installed and linked.
4. Build & Run.

## How to Play
1. The game is played on a hexagonal board:
   - Select one of your pieces.
   - You can **clone** it to an adjacent cell or **jump** two cells away.
   - After moving, all opponent’s pieces adjacent to the landing cell become yours.
2. Players alternate turns until no more moves are possible.
3. The winner is the player controlling the majority of the board.

## Screenshots

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
#### Menu

<img width="801" height="627" alt="image" src="https://github.com/user-attachments/assets/63ce52e9-91eb-415b-afc8-9cd21080fe80" />


#### In game footage

<img width="798" height="628" alt="image" src="https://github.com/user-attachments/assets/a281d3ec-568d-4e41-8ccb-15ad54b4206c" />
<img width="799" height="629" alt="image" src="https://github.com/user-attachments/assets/38d61eca-5f17-4e0e-aa62-4c26495741b9" />
<img width="800" height="629" alt="image" src="https://github.com/user-attachments/assets/3106139a-31a4-4b7d-ae74-041801d6d32d" />


#### Best games

<img width="799" height="629" alt="image" src="https://github.com/user-attachments/assets/df8933a7-cf35-49bc-8ca1-33c5400f56b7" />





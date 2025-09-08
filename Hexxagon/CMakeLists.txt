cmake_minimum_required(VERSION 3.30)
project(Hexxagon)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 11.1.4
)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(fmt SFML)

add_executable(Hexxagon main.cpp
        Core/Board/Field.cpp
        Core/Board/Field.hpp
        Core/Board/BoardCreator.cpp
        Core/Board/BoardCreator.hpp
        Core/Board/Board.cpp
        Core/Board/Board.hpp
        Core/TurnAndMoves/Turn.cpp
        Core/TurnAndMoves/Turn.hpp
        Core/TurnAndMoves/Move.cpp
        Core/TurnAndMoves/Move.hpp
        GUI/Score.cpp
        GUI/Score.hpp
        Core/TurnAndMoves/Game.cpp
        Core/TurnAndMoves/Game.hpp
        Core/TurnAndMoves/Ai.cpp
        Core/TurnAndMoves/Ai.hpp
        GUI/Button.cpp
        GUI/Button.hpp
        GUI/Interface.cpp
        GUI/Interface.hpp
)
target_link_libraries(Hexxagon fmt SFML::Graphics)
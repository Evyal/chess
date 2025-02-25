#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "constants.h"
#include "move.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Vector2.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBoxSlider.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <string>
#include <vector>
class Game {

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////

private:
  sf::RenderWindow window;
  tgui::Gui gui;

  Board board;

  // Move Pieces on click
  tgui::Button::Ptr tiles[constants::squares][constants::squares];
  std::pair<int, int> selectedTile{-1, -1};

  // Track Turn
  bool whiteTurn = true;
  tgui::Label::Ptr turnLabel;

  // Track Moves
  std::vector<Move> moveLog{};
  size_t moveNumber{0};

  // Track Notation
  std::vector<std::string> notationLog{};

  // Rotate Board
  bool isRotated{false};

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////

public:
  Game();
  void setup();
  void run();

  void createButtons();
  void rotateBoard();
  void handleButtonClick(int row, int col);
  void highlightSelection(int row, int col, bool highlight);

  void switchTurn();

  void handleMove(const Move &move);
  void logMove(const Move &move);

  std::string notation(int row, int col);
  void logNotation(const Move &move);
  void fileNotation(const Move &move);
  void printMoveParameters();

  void undoMove();
  void redoMove();

  void createTurnLabel();
  void updateTurnLabel();
};

#endif

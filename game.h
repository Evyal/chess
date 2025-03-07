#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "constants.h"
#include "graphics.h"
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

  Board board;                 // Logic handler
  BoardGraphics boardGraphics; // Ggraphics handler

  // Move Pieces on click
  tgui::Button::Ptr tiles[constants::squares][constants::squares];
  std::pair<int, int> selectedTile{-1, -1};

  // Track Turn
  bool whiteTurn = true;
  tgui::Label::Ptr turnLabel;

  // Track Moves
  std::vector<Move> moveLog{};
  size_t moveNumber{0};

  // Rotate Board
  bool isRotated{false};

  // Result
  int result{-2};

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////

public:
  Game();
  void setup();
  void run();

  void createButtons();

  ////////////////////////////////////////////////////////////////////////////////////////////////////

  void handleButtonClick(int row, int col);
  void highlightSelection(int row, int col, bool highlight);
  void switchTurn();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // MOVES

  void handleMove(const Move &move);
  void logMove(const Move &move);
  void handleCastling(bool kingSide);
  void promotePawn(Move &move);

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // GO BACK with MOVES

  void undoMove();
  void redoMove();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // NOTATION

  std::string notationPGN(const Move &move);
  void notationFEN();
  void fileNotation();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // DISPLAY TURN

  void createTurnLabel();
  void updateTurnLabel();
  void rotateBoard();
};

#endif

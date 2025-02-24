#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Vector2.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBoxSlider.hpp>
#include <TGUI/Widgets/Label.hpp>
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

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////

public:
  Game();
  void setup();
  void run();

  void createButtons();
  void handleButtonClick(int row, int col);
  void highlightSelection(int row, int col, bool highlight, bool unhighlight);

  void handleMove(int row, int col);
  void movePiece(const Move &move);
  void capturePiece(const Move &move);
  bool handleCastling(const Move &move);
  void switchTurn();

  std::string notation(int row, int col);

  void createTurnLabel();
  void updateTurnLabel();
};

#endif

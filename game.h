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

  tgui::Button::Ptr tiles[constants::squares][constants::squares];
  std::pair<int, int> selectedTile{-1, -1};

  bool whiteTurn = true; // Track turn

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////

public:
  Game();
  void setup();
  void run();

  void createButtons();
  void handleButtonClick(int row, int col);
};

#endif

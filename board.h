#ifndef BOARD_H
#define BOARD_H

#include "constants.hpp"
#include "piece.h"
#include <vector>

class Board {
private:
  int board_[constants::boardSize][constants::boardSize] = {
      {}, {}, {}, {}, {}, {}, {}, {}, {},
  };

  sf::RenderWindow *window;
  sf::Texture pieceTextures[7][2]; // 6 types of pieces, 2 colors
  std::map<int, sf::Sprite> pieceSprites;

  void loadTextures();
  void initializeBoard();

public:
  Board(sf::RenderWindow *win);
  void draw();

  // Board();
  // ~Board();

  // void display();
  // bool movePiece(int startX, int startY, int endX, int endY);
};

#endif

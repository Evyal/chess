#ifndef BOARD_H
#define BOARD_H

#include "constants.hpp"
#include "piece.h"
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Board {
private:
  int board_[constants::squares][constants::squares] = {{}, {}, {}, {},
                                                        {}, {}, {}, {}};

  sf::RenderWindow *window;

  sf::Texture pieceTextures[7][2]; // 6 types of pieces, 2 colors
  std::map<int, sf::Sprite> pieceSprites;

  sf::Texture background;
  sf::Texture board;
  sf::Sprite backgroundSprite;
  sf::Sprite boardSprite;

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

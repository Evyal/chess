#ifndef BOARD_H
#define BOARD_H

#include "constants.hpp"
#include "piece.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>

class Board {
private:
  Piece *board[constants::squares][constants::squares] = {{}, {}, {}, {},
                                                          {}, {}, {}, {}};
  sf::RenderWindow *window;

  // BOARD TEXTURES
  sf::Texture backgroundTexture;
  sf::Texture boardTexture;
  sf::Sprite backgroundSprite;
  sf::Sprite boardSprite;

  // PIECE TEXTURES
  std::map<int, sf::Texture> pieceTextures;
  std::map<int, sf::Sprite> pieceSprites;

  void loadTextures();
  void loadPieceTextures(); // New function to load piece images

public:
  // CONSTRUCTORS and DESTRUCTORS

  Board();
  Board(sf::RenderWindow *win);
  ~Board();

  // MOVING PIECES

  Piece *getPiece(int x, int y);
  void movePiece(int startX, int startY, int endX, int endY);

  // GRAPHICS

  void initializeBoard();
  void draw();
};

#endif

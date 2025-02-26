#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <utility>

class Board {
private:
  Piece *board[constants::squares][constants::squares] = {{}, {}, {}, {},
                                                          {}, {}, {}, {}};

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
  ~Board();

  // MOVING PIECES

  Piece *getPiece(int x, int y) const;
  void setPiece(int col, int row, Piece *piece);
  void movePiece(int startX, int startY, int endX, int endY);

  bool isPathClear(int startX, int startY, int endX, int endY) const;
  bool isSquareUnderAttack(int x, int y, bool isWhite) const;
  bool isControllingSquare(int row, int col, Piece *piece) const;
  Rook *getRookForCastling(int KingX, int kingY, bool kingSide,
                                   bool kingColor) const;

  // GRAPHICS

  void initializeBoard();
  void setup960();
  void draw(sf::RenderWindow &window, bool rotated);
};

#endif

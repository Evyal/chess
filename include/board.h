#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "piece.h"

class Board {
private:
  // Logical board: each square holds a pointer to a Piece (or nullptr for no piece)
  Piece *board[constants::squares][constants::squares] = {{}, {}, {}, {},
                                                          {}, {}, {}, {}};

public:
  Board();
  ~Board();

  // Board setup functions
  void initializeBoard();
  void setup960();

  // Get set move pieces
  Piece *getPiece(int x, int y) const;
  void setPiece(int col, int row, Piece *piece);
  void movePiece(int startX, int startY, int endX, int endY);

  // Game logic functions
  bool isPathClear(int startX, int startY, int endX, int endY) const;
  bool isSquareUnderAttack(int x, int y, bool isWhite) const;
  Rook *getRookForCastling(int KingX, int kingY, bool kingSide,
                           bool kingColor) const;
  bool isControllingSquare(int row, int col, Piece *piece) const;
};

#endif // BOARD_H

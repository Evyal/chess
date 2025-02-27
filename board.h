#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "piece.h"

class Board {
private:
  // Logical board: each square holds a pointer to a Piece (or nullptr)
  Piece *board[constants::squares][constants::squares] = {{}, {}, {}, {},
                                                          {}, {}, {}, {}};

public:
  Board();
  ~Board();

  // Game logic functions
  Piece *getPiece(int x, int y) const;
  void setPiece(int col, int row, Piece *piece);
  void movePiece(int startX, int startY, int endX, int endY);

  bool isPathClear(int startX, int startY, int endX, int endY) const;
  bool isSquareUnderAttack(int x, int y, bool isWhite) const;
  bool isControllingSquare(int row, int col, Piece *piece) const;
  Rook *getRookForCastling(int KingX, int kingY, bool kingSide,
                           bool kingColor) const;

  // Board setup functions (logic)
  void initializeBoard();
  void setup960();
};

#endif // BOARD_H

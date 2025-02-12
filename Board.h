#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <vector>

class Board {
private:
  std::vector<std::vector<Piece *>> squares; // 8x8 grid

public:
  Board();
  ~Board(); // Destructor to clean up dynamically allocated pieces
  void display();
  bool movePiece(int startX, int startY, int endX, int endY);
  // Add other methods as needed (e.g., checking for check/checkmate)
};

#endif

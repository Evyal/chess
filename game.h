#ifndef GAME_H
#define GAME_H

#include "board.h"
class Game {
private:
  Board board;
  bool whiteTurn;

public:
  Game() : whiteTurn(true) {}

  // TURN LOGIC

  void switchTurn();
  bool isWhiteTurn() const;
  void makeMove(int startX, int startY, int endX, int endY);

  bool isCheckmate() {
    // Implement checkmate logic by:
    // 1. Check if the King is in check
    // 2. Check if the King can move to a safe square
    // 3. Check if other pieces can block or capture attacking pieces

    return false; // Placeholder, implement checkmate detection
  }

  // Add other game logic like turn management, detecting check, etc.
};

#endif

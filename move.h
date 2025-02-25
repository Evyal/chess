#ifndef MOVE_H
#define MOVE_H

#include "piece.h"

struct Move {
  int startX{};
  int startY{};
  int endX{};
  int endY{};
  Piece *pieceStart{};
  Piece *pieceEnd{};

  bool isMoving{false};
  bool isCapture{false};
  bool isShortCastle{false};
  bool isLongCastle{false};
  bool isFirstTimeMoving{true};
};

bool areMovesEqual(const Move &move1, const Move &move2);

#endif
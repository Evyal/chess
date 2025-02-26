#ifndef MOVE_H
#define MOVE_H

#include "piece.h"

struct Move {
  int startX{-1};
  int startY{-1};
  Piece *pieceStart{};

  int endX{-1};
  int endY{-1};
  Piece *pieceEnd{};

  bool isMoving{false};
  bool isCapture{false};
  bool isShortCastle{false};
  bool isLongCastle{false};
  bool isFirstTimeMoving{true};
};

bool areMovesEqual(const Move &move1, const Move &move2);

#endif
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

  bool isMoving{};
  bool isCapture{};
  bool isShortCastle{};
  bool isLongCastle{};
  bool isFirstTimeMoving{};
};

#endif
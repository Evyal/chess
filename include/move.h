#ifndef MOVE_H
#define MOVE_H

#include "piece.h"

struct Move {
  // START PIECE
  int startX{-1};
  int startY{-1};
  Piece *pieceStart{};

  // END PIECE
  int endX{-1};
  int endY{-1};
  Piece *pieceEnd{};

  // TYPE of MOVE and ATTRIBUTES
  bool isMoving{false};
  bool isCapture{false};
  bool isShortCastle{false};
  bool isLongCastle{false};
  bool isFirstTimeMoving{true};
  bool isPromotion{false};

  // EVENTUAL PROMOTION PIECE
  Piece *promotionPiece{};
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// CHECK if two MOVES ARE EQUAL

bool areMovesEqual(const Move &move1, const Move &move2);

#endif
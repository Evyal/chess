#include "move.h"

bool areMovesEqual(const Move &move1, const Move &move2) {
  if (move1.startX != move2.startX) {
    return false;
  }
  if (move1.startY != move2.startY) {
    return false;
  }
  if (move1.endX != move2.endX) {
    return false;
  }
  if (move1.endY != move2.endY) {
    return false;
  }
  if (move1.pieceStart != move2.pieceStart) {
    return false;
  }
  if (move1.pieceEnd != move2.pieceEnd) {
    return false;
  }
  if (move1.isMoving != move2.isMoving) {
    return false;
  }
  if (move1.isCapture != move2.isCapture) {
    return false;
  }
  if (move1.isFirstTimeMoving != move2.isFirstTimeMoving) {
    return false;
  }
  if (move1.isShortCastle != move2.isShortCastle) {
    return false;
  }
  if (move1.isLongCastle != move2.isLongCastle) {
    return false;
  }
  if (move1.isPromotion != move2.isPromotion) {
    return false;
  }
  if (move1.isPromotion != move2.isPromotion) {
    return false;
  }
  if (move1.promotionPiece != move2.promotionPiece) {
    return false;
  }
  return true;
}

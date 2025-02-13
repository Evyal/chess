#include "piece.h"
#include <cmath>

bool Pawn::isValidMove(int startX, int startY, int endX, int endY) {
  int direction = isWhite ? 1 : -1; // White moves up, black moves down

  // Standard move (one step forward)
  if (endX == startX + direction && endY == startY)
    return true;

  // First move can be two steps forward
  if ((startX == 2 && isWhite) || (startX == 7 && !isWhite)) {
    if (endX == startX + 2 * direction && endY == startY)
      return true;
  }

  // Diagonal capture
  if (endX == startX + direction &&
      (endY == startY + 1 || endY == startY - 1)) {
    return true;
  }

  return false;
}

int Pawn::getType() {
  if (isWhitePiece()) {
    return 1;
  }
  return -1;
}

bool Rook::isValidMove(int startX, int startY, int endX, int endY) {
  return (startX == endX || startY == endY);
}

int Rook::getType() {
  if (isWhitePiece()) {
    return 2;
  }
  return -2;
}

bool Knight::isValidMove(int startX, int startY, int endX, int endY) {
  int dx = abs(endX - startX);
  int dy = abs(endY - startY);
  return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

int Knight::getType() {
  if (isWhitePiece()) {
    return 3;
  }
  return -3;
}

bool Bishop::isValidMove(int startX, int startY, int endX, int endY) {
  return abs(endX - startX) == abs(endY - startY);
}

int Bishop::getType() {
  if (isWhitePiece()) {
    return 4;
  }
  return -4;
}

bool Queen::isValidMove(int startX, int startY, int endX, int endY) {
  return ((startX == endX || startY == endY) ||
          abs(endX - startX) == abs(endY - startY));
}

int Queen::getType() {
  if (isWhitePiece()) {
    return 5;
  }
  return -5;
}

bool King::isValidMove(int startX, int startY, int endX, int endY) {
  int dx = abs(endX - startX);
  int dy = abs(endY - startY);
  return (dx <= 1 && dy <= 1);
}

int King::getType() {
  if (isWhitePiece()) {
    return 6;
  }
  return -6;
}
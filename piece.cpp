#include "piece.h"
#include <cmath>

// Pawn implementation (example)
bool Pawn::isValidMove(int startX, int startY, int endX, int endY) {
  // Implement pawn movement logic
  return false; // Placeholder
}

// Rook implementation (example)
bool Rook::isValidMove(int startX, int startY, int endX, int endY) {
  return (startX == endX || startY == endY);
}

// Bishop implementation
bool Bishop::isValidMove(int startX, int startY, int endX, int endY) {
  return abs(startX - endX) == abs(startY - endY);
}

// Knight implementation
bool Knight::isValidMove(int startX, int startY, int endX, int endY) {
  int dx = abs(startX - endX);
  int dy = abs(startY - endY);
  return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

// Queen implementation
bool Queen::isValidMove(int startX, int startY, int endX, int endY) {
  return (startX == endX || startY == endY ||
          abs(startX - endX) == abs(startY - endY));
}
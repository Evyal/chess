#include "piece.h"
#include "board.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////////////////

Piece::Piece(bool white) : isWhite(white) {}

Piece::Piece(bool white, std::pair<int, int> startingPos)
    : isWhite(white), startingPos_(startingPos) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Piece::markAsMoved(bool hasMoved) { hasMoved_ = hasMoved; }
bool Piece::hasMovedBefore() const { return hasMoved_; }
std::pair<int, int> Piece::getStartingPosition() {return startingPos_;}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// PAWN

Pawn::Pawn(bool white) : Piece(white) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Pawn::isValidMove(int startX, int startY, int endX, int endY,
                       const Board &board) {
  int direction = isWhite ? 1 : -1; // White moves up, Black moves down

  // Standard move (one step forward) - only allowed if target is empty
  if (endY == startY + direction && endX == startX &&
      board.getPiece(endX, endY) == nullptr) {
    return true;
  }

  // First move can be two steps forward, if the path is clear
  if ((startY == 1 && isWhite) || (startY == 6 && !isWhite)) {
    if (endY == startY + 2 * direction && endX == startX &&
        board.getPiece(startX, startY + direction) == nullptr &&
        board.getPiece(endX, endY) == nullptr) {
      return true;
    }
  }

  // Diagonal capture - only allowed if there's an opponent piece
  if (endY == startY + direction &&
      (endX == startX + 1 || endX == startX - 1)) {
    Piece *target = board.getPiece(endX, endY);
    if (target &&
        target->isWhitePiece() != isWhite) { // Capturing an opponent piece
      return true;
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Pawn::getType() {
  if (isWhitePiece()) {
    return 1;
  }
  return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Pawn::getSymbolPNG() const { return 'P'; }

////////////////////////////////////////////////////////////////////////////////////////////////////

char Pawn::getSymbolFEN() const {
  if (isWhite == true) {
    return 'P';
  } else {
    return 'p';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// ROOK

Rook::Rook(bool white) : Piece(white) {}

Rook::Rook(bool white, std::pair<int, int> startingPos)
    : Piece(white, startingPos) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Rook::isValidMove(int startX, int startY, int endX, int endY,
                       const Board &board) {
  if (startX != endX && startY != endY)
    return false;
  return board.isPathClear(startX, startY, endX, endY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Rook::getType() {
  if (isWhitePiece()) {
    return 2;
  }
  return -2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Rook::getSymbolPNG() const { return 'R'; }

////////////////////////////////////////////////////////////////////////////////////////////////////

char Rook::getSymbolFEN() const {
  if (isWhite == true) {
    return 'R';
  } else {
    return 'r';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// KNIGHT

Knight::Knight(bool white) : Piece(white) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Knight::isValidMove(int startX, int startY, int endX, int endY,
                         const Board &board) {
  int dx = abs(endX - startX);
  int dy = abs(endY - startY);
  return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Knight::getType() {
  if (isWhitePiece()) {
    return 3;
  }
  return -3;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Knight::getSymbolPNG() const { return 'N'; }

////////////////////////////////////////////////////////////////////////////////////////////////////

char Knight::getSymbolFEN() const {
  if (isWhite == true) {
    return 'N';
  } else {
    return 'n';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// BISHOP

Bishop::Bishop(bool white) : Piece(white) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Bishop::isValidMove(int startX, int startY, int endX, int endY,
                         const Board &board) {
  if (abs(endX - startX) != abs(endY - startY))
    return false;
  return board.isPathClear(startX, startY, endX, endY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Bishop::getType() {
  if (isWhitePiece()) {
    return 4;
  }
  return -4;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Bishop::getSymbolPNG() const { return 'B'; }

////////////////////////////////////////////////////////////////////////////////////////////////////

char Bishop::getSymbolFEN() const {
  if (isWhite == true) {
    return 'B';
  } else {
    return 'b';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// QUEEN

Queen::Queen(bool white) : Piece(white) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Queen::isValidMove(int startX, int startY, int endX, int endY,
                        const Board &board) {
  if ((startX == endX || startY == endY) ||
      (abs(endX - startX) == abs(endY - startY))) {
    return board.isPathClear(startX, startY, endX, endY);
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Queen::getType() {
  if (isWhitePiece()) {
    return 5;
  }
  return -5;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Queen::getSymbolPNG() const { return 'Q'; }

////////////////////////////////////////////////////////////////////////////////////////////////////

char Queen::getSymbolFEN() const {
  if (isWhite == true) {
    return 'Q';
  } else {
    return 'q';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// KING

King::King(bool white) : Piece(white) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool King::isValidMove(int startX, int startY, int endX, int endY,
                       const Board &board) {
  int dx = abs(endX - startX);
  int dy = abs(endY - startY);

  // Normal King move (one square in any direction)
  if (dx <= 1 && dy <= 1) {
    return true;
  }

  // Castling logic
  if (!hasMovedBefore() &&
      startY == endY) { // King moves horizontally for castling
    if (endX == startX + 2 ||
        endX == startX - 2) { // Castling happens two squares left or right
      int rookX =
          (endX > startX) ? 7 : 0; // Determine which rook (right or left)
      Piece *rook = board.getPiece(rookX, startY);

      // Check if a rook is present and hasn't moved
      if (rook && (rook->getType() == 2 || rook->getType() == -2) &&
          !rook->hasMovedBefore()) {

        // Ensure no pieces are between king and rook
        int step = (endX > startX) ? 1 : -1;
        for (int x = startX + step; x != rookX; x += step) {
          if (board.getPiece(x, startY)) {
            return false; // Pieces are blocking
          }
        }

        // Ensure the king is not in check, does not move through check, and
        // does not end in check
        for (int x = startX; x != endX + step; x += step) {
          if (board.isSquareUnderAttack(x, startY, isWhitePiece())) {
            return false; // King moves through or into check
          }
        }

        return true; // Castling is allowed
      }
    }
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int King::getType() {
  if (isWhitePiece()) {
    return 6;
  }
  return -6;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char King::getSymbolPNG() const { return 'K'; }

////////////////////////////////////////////////////////////////////////////////////////////////////

char King::getSymbolFEN() const {
  if (isWhite == true) {
    return 'K';
  } else {
    return 'k';
  }
}
#include "board.h"
#include "constants.h"
#include "piece.h"
#include "random.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

Board::Board() {
  // initializeBoard();

  setup960();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Board::~Board() {
  for (int i = 0; i < constants::squares; i++) {
    for (int j = 0; j < constants::squares; j++) {
      delete board[i][j];
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void Board::initializeBoard() {
  // Place Pawns
  for (int i = 0; i < 8; i++) {
    board[i][1] = new Pawn(true);  // White Pawn
    board[i][6] = new Pawn(false); // Black Pawn
  }

  // Place Rooks
  board[0][0] = new Rook(true, {0, 0});
  board[7][0] = new Rook(true, {7, 0});
  board[0][7] = new Rook(false, {0, 7});
  board[7][7] = new Rook(false, {7, 7});

  // Place Knights
  board[1][0] = new Knight(true);
  board[6][0] = new Knight(true);
  board[1][7] = new Knight(false);
  board[6][7] = new Knight(false);

  // Place Bishops
  board[2][0] = new Bishop(true);
  board[5][0] = new Bishop(true);
  board[2][7] = new Bishop(false);
  board[5][7] = new Bishop(false);

  // Place Kings
  board[4][0] = new King(true, {4, 0});
  board[4][7] = new King(false, {4, 7});

  // Place Queens
  board[3][0] = new Queen(true);
  board[3][7] = new Queen(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Board::setup960() {
  // Place Pawns for both sides
  for (int i = 0; i < 8; i++) {
    board[i][1] = new Pawn(true);  // White Pawn
    board[i][6] = new Pawn(false); // Black Pawn
  }

  int kingX = randomInt(1, 6);
  setPiece(kingX, 0, new King(true, {kingX, 0}));
  setPiece(kingX, 7, new King(false, {kingX, 7}));

  int RookXQ = randomInt(0, kingX - 1);
  int RookXK = randomInt(kingX + 1, 7);

  setPiece(RookXQ, 0, new Rook(true, {RookXQ, 0})); // Queenside rook
  setPiece(RookXK, 0, new Rook(true, {RookXK, 0})); // Kingside rook

  setPiece(RookXQ, 7, new Rook(false, {RookXQ, 7})); // Queenside rook
  setPiece(RookXK, 7, new Rook(false, {RookXK, 7})); // Kingside rook

  int Bishop1X = randomInt(0, 7);
  while (getPiece(Bishop1X, 0)) {
    Bishop1X = (Bishop1X + 1) % 8;
  }
  setPiece(Bishop1X, 0, new Bishop(true));
  setPiece(Bishop1X, 7, new Bishop(false));

  int Bishop2X = randomInt(0, 7);
  if (Bishop2X % 2 == Bishop1X % 2) {
    Bishop2X++;
  }
  while (getPiece(Bishop2X, 0)) {
    Bishop2X = (Bishop2X + 2) % 8;
  }
  setPiece(Bishop2X, 0, new Bishop(true));
  setPiece(Bishop2X, 7, new Bishop(false));

  int Knight1X = randomInt(0, 7);
  while (getPiece(Knight1X, 0)) {
    Knight1X = (Knight1X + 1) % 8;
  }
  setPiece(Knight1X, 0, new Knight(true));
  setPiece(Knight1X, 7, new Knight(false));

  int Knight2X = randomInt(0, 7);
  while (getPiece(Knight2X, 0)) {
    Knight2X = (Knight2X + 1) % 8;
  }
  setPiece(Knight2X, 0, new Knight(true));
  setPiece(Knight2X, 7, new Knight(false));

  int QueenX = randomInt(0, 7);
  while (getPiece(QueenX, 0)) {
    QueenX = (QueenX + 1) % 8;
  }
  setPiece(QueenX, 0, new Queen(true));
  setPiece(QueenX, 7, new Queen(false));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void Board::movePiece(int startX, int startY, int endX, int endY) {
  if (startX < 0 || startX >= constants::squares || startY < 0 ||
      startY >= constants::squares || endX < 0 || endX >= constants::squares ||
      endY < 0 || endY >= constants::squares) {
    std::cerr << "movePiece: Invalid coordinates!" << std::endl;
    return;
  }

  if (!board[startX][startY]) {
    std::cerr << "movePiece: No piece at (" << startX << ", " << startY << ")"
              << std::endl;
    return;
  }

  board[endX][endY] = board[startX][startY];
  board[startX][startY] = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Piece *Board::getPiece(int x, int y) const { return board[x][y]; }

////////////////////////////////////////////////////////////////////////////////////////////////////

void Board::setPiece(int col, int row, Piece *piece) {
  board[col][row] = piece;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

bool Board::isPathClear(int startX, int startY, int endX, int endY) const {
  int dx = (endX - startX);
  int dy = (endY - startY);
  int stepX = (dx == 0) ? 0 : (dx / std::abs(dx));
  int stepY = (dy == 0) ? 0 : (dy / std::abs(dy));
  int x = startX + stepX;
  int y = startY + stepY;

  while (x != endX || y != endY) {
    if (getPiece(x, y) != nullptr) {
      return false;
    }
    x += stepX;
    y += stepY;
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Board::isSquareUnderAttack(int x, int y, bool isWhite) const {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      Piece *piece = getPiece(i, j);
      if (piece && (piece->isWhitePiece() != isWhite)) {
        if (piece->isValidMove(i, j, x, y, *this)) {
          return true;
        }
      }
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Rook *Board::getRookForCastling(int KingX, int KingY, bool kingSide,
                                bool kingColor) const {
  for (int x = 0; x < 8; x++) {
    Piece *p = getPiece(x, KingY);
    if (p && p->getType() == (kingColor ? 2 : -2) && !p->hasMovedBefore()) {
      if (kingSide && x > KingX)
        return dynamic_cast<Rook *>(p);
      if (!kingSide && x < KingX)
        return dynamic_cast<Rook *>(p);
    }
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// bool Board::isControllingSquare(int row, int col, Piece *piece) const {}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

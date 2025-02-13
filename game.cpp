#include "game.h"

void Game::switchTurn() { whiteTurn = !whiteTurn; }

bool Game::isWhiteTurn() const { return whiteTurn; }

void Game::makeMove(int startX, int startY, int endX, int endY) {
  // Check if the piece belongs to the current player
  Piece *piece = board.getPiece(startX, startY);
  if (piece && piece->isWhitePiece() == whiteTurn) {
    // Check if the move is valid for the current piece
    if (piece->isValidMove(startX, startY, endX, endY)) {
      board.movePiece(startX, startY, endX, endY);
      switchTurn(); // Switch turn after a valid move
    }
  }
}
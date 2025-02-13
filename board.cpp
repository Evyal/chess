#include "board.h"
#include "constants.hpp"
#include "piece.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

Board::Board(sf::RenderWindow *win) : window(win) {
  loadTextures();
  loadPieceTextures();
  initializeBoard();
}

Board::~Board() {
  // Free dynamically allocated memory for pieces
  for (int i = 0; i < constants::squares; i++) {
    for (int j = 0; j < constants::squares; j++) {
      delete board[i][j]; // Delete each dynamically allocated Piece
    }
  }
}

void Board::loadTextures() {
  if (!backgroundTexture.loadFromFile("../bases/background.png")) {
    std::cerr << "Failed to load " << "../bases/background.png" << std::endl;
  }

  if (!boardTexture.loadFromFile("../bases/board.png")) {
    std::cerr << "Failed to load " << "../bases/board.png" << std::endl;
  }

  backgroundSprite.setTexture(backgroundTexture);
  boardSprite.setTexture(boardTexture);
}

void Board::loadPieceTextures() {
  std::map<int, std::string> pieceFiles = {
      {1, "wp.png"},  {2, "wr.png"},  {3, "wn.png"},  {4, "wb.png"},
      {5, "wq.png"},  {6, "wk.png"},  {-1, "bp.png"}, {-2, "br.png"},
      {-3, "bn.png"}, {-4, "bb.png"}, {-5, "bq.png"}, {-6, "bk.png"}};

  for (const auto &pair : pieceFiles) {
    int pieceType = pair.first;
    std::string filename = "../bases/" + pair.second;

    if (!pieceTextures[pieceType].loadFromFile(filename)) {
      std::cerr << "Failed to load " << filename << std::endl;
    } else {
      pieceSprites[pieceType].setTexture(pieceTextures[pieceType]);
    }
  }
}

void Board::initializeBoard() {
  for (int i{0}; i < 8; i++) {
    board[1][i] = new Pawn(true);  // White Pawn
    board[6][i] = new Pawn(false); // Black Pawn
  }

  // Place Rooks
  board[0][0] = new Rook(true);
  board[0][7] = new Rook(true);
  board[7][0] = new Rook(false);
  board[7][7] = new Rook(false);

  // Place Knights
  board[0][1] = new Knight(true);
  board[0][6] = new Knight(true);
  board[7][1] = new Knight(false);
  board[7][6] = new Knight(false);

  // Place Bishops
  board[0][2] = new Bishop(true);
  board[0][5] = new Bishop(true);
  board[7][2] = new Bishop(false);
  board[7][5] = new Bishop(false);

  // Place Kings
  board[0][3] = new King(true);
  board[7][3] = new King(false);

  // Place Queens
  board[0][4] = new Queen(true);
  board[7][4] = new Queen(false);
}

void Board::draw() {

  backgroundSprite.setPosition(0, 0);
  backgroundSprite.setScale(
      constants::windowWidth / backgroundSprite.getLocalBounds().width,
      constants::windowHeight / backgroundSprite.getLocalBounds().height);
  window->draw(backgroundSprite);

  boardSprite.setPosition(constants::marginSize, constants::marginSize);
  boardSprite.setScale(
      constants::boardWidth / boardSprite.getLocalBounds().width,
      constants::boardHeight / boardSprite.getLocalBounds().height);
  window->draw(boardSprite);

  for (int i = 0; i < constants::squares; i++) {
    for (int j = 0; j < constants::squares; j++) {
      if (board[i][j] != nullptr) {

        int pieceType =
            board[i][j]->getType(); // Assuming you have a getType() function

        if (pieceSprites.find(pieceType) != pieceSprites.end()) {
          sf::Sprite &sprite = pieceSprites[pieceType];
          sprite.setPosition(static_cast<float>(j) * constants::tileSize +
                                 constants::marginSize,
                             static_cast<float>(i) * constants::tileSize +
                                 constants::marginSize);
          sprite.setScale(
              constants::tileSize / sprite.getLocalBounds().width,
              constants::tileSize /
                  sprite.getLocalBounds().height); // Adjust scaling if needed
          window->draw(sprite);
        }
      }
    }
  }
}

Piece* Board::getPiece(int x, int y) {
  if (x >= 0 && x < constants::squares && y >= 0 && y < constants::squares) {
    return board[x][y];
  }
  return nullptr;
}

// Move a piece from one position to another
void Board::movePiece(int startX, int startY, int endX, int endY) {
  Piece *piece = board[startX][startY];
  if (piece) {
    board[endX][endY] = piece;
    board[startX][startY] = nullptr;
  }
}
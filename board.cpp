#include "board.h"
#include "constants.hpp"
#include <SFML/Graphics/Color.hpp>
#include <iostream>

Board::Board(sf::RenderWindow *win) : window(win) {
  loadTextures();
  initializeBoard();
}

void Board::loadTextures() {
  std::string pieceNames[] = {"pawn",  "rook", "knight", "bishop",
                              "queen", "king", "jester"};
  std::string colors[] = {"w", "b"}; // White and Black

  for (int i{0}; i < 7; i++) {
    for (int j = 0; j < 2; j++) {
      std::string filename =
          "../images/" + pieceNames[i] + "_" + colors[j] + ".png";
      if (!pieceTextures[i][j].loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename << std::endl;
      }
    }
  }

  // Map integer values to sprites
  for (int i{1}; i < 8; i++) {
    pieceSprites[i].setTexture(pieceTextures[i - 1][0]);  // White pieces
    pieceSprites[-i].setTexture(pieceTextures[i - 1][1]); // Black pieces
  }
}

void Board::initializeBoard() {
  for (int i = 0; i < constants::boardSize; i++) {
    for (int j = 0; j < constants::boardSize; j++) {
      board_[i][j] = 0;
    }
  }

  // Setup chess pieces (White = Positive, Black = Negative)
  for (int i = 0; i <= 8; i++) {
    board_[1][i] = 1;  // White pawn
    board_[7][i] = -1; // Black pawn
  }

  board_[0][0] = board_[0][8] = 2;  // White rooks
  board_[8][0] = board_[8][8] = -2; // Black rooks

  // KNIGHTS
  board_[0][1] = 3;
  board_[0][6] = 3;
  board_[8][1] = -3;
  board_[8][6] = -3;

  // BISHOPS
  board_[0][2] = 4;
  board_[0][7] = 4;
  board_[8][2] = -4;
  board_[8][7] = -4;

  board_[0][3] = 5;  // White queen
  board_[8][3] = -5; // Black queen

  board_[0][4] = 6;  // White king
  board_[8][4] = -6; // Black king

  board_[0][5] = 7;
  board_[8][5] = -7;
}

void Board::draw() {
  sf::RectangleShape tile(
      sf::Vector2f(constants::tileSize, constants::tileSize));

  for (int i = 0; i < constants::boardSize; i++) {
    for (int j = 0; j < constants::boardSize; j++) {
      // if (i == 0 || j == 0)
      //   continue; // Skip labels

      tile.setFillColor((i + j) % 2 == 0 ? sf::Color::White
                                         : sf::Color(150, 75, 0));
      tile.setPosition(static_cast<float>(j) * constants::tileSize,
                       static_cast<float>(i) * constants::tileSize);
      window->draw(tile);

      // Draw pieces
      int piece = board_[i][j];
      if (piece != 0) {
        sf::Sprite &sprite = pieceSprites[piece];
        sprite.setPosition(static_cast<float>(j) * constants::tileSize,
                           static_cast<float>(i) * constants::tileSize);
        sprite.setScale(constants::tileSize / sprite.getLocalBounds().width,
                        constants::tileSize / sprite.getLocalBounds().height);
        window->draw(sprite);
      }
    }
  }
}

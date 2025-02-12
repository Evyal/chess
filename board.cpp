#include "board.h"
#include "constants.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

Board::Board(sf::RenderWindow *win) : window(win) {
  loadTextures();
  initializeBoard();
}

void Board::loadTextures() {
  std::string pieceNames[] = {"p", "r", "n", "b", "q", "k"};
  std::string colors[] = {"w", "b"}; // White and Black

  for (int i{0}; i < 6; i++) {
    for (int j = 0; j < 2; j++) {
      std::string filename = "../bases/" + colors[j] + pieceNames[i] + ".png";
      if (!pieceTextures[i][j].loadFromFile(filename)) {
        std::cerr << "Failed to load " << filename << std::endl;
      }
    }
  }

  // Map integer values to sprites
  for (int i{1}; i < 7; i++) {
    pieceSprites[i].setTexture(pieceTextures[i - 1][0]);  // White pieces
    pieceSprites[-i].setTexture(pieceTextures[i - 1][1]); // Black pieces
  }

  if (!background.loadFromFile("../bases/background.png")) {
    std::cerr << "Failed to load " << "../bases/background.png" << std::endl;
  }

  if (!board.loadFromFile("../bases/board.png")) {
    std::cerr << "Failed to load " << "../bases/board.png" << std::endl;
  }

  backgroundSprite.setTexture(background);
  boardSprite.setTexture(board);
}

void Board::initializeBoard() {
  for (size_t i{0}; i < constants::squares; i++) {
    for (size_t j{0}; j < constants::squares; j++) {
      board_[i][j] = 0;
    }
  }

  // Setup chess pieces (White = Positive, Black = Negative)
  for (int i = 0; i < 8; i++) {
    board_[1][i] = 1;  // White pawn
    board_[6][i] = -1; // Black pawn
  }

  board_[0][0] = board_[0][7] = 2;  // White rooks
  board_[7][0] = board_[7][7] = -2; // Black rooks

  // KNIGHTS
  board_[0][1] = 3;
  board_[0][6] = 3;
  board_[7][1] = -3;
  board_[7][6] = -3;

  // BISHOPS
  board_[0][2] = 4;
  board_[0][5] = 4;
  board_[7][2] = -4;
  board_[7][5] = -4;

  board_[0][4] = 5;  // White queen
  board_[7][4] = -5; // Black queen

  board_[0][3] = 6;  // White king
  board_[7][3] = -6; // Black king
}

void Board::draw() {

  backgroundSprite.setPosition(0, 0);
  boardSprite.setPosition(constants::marginSize, constants::marginSize);

  backgroundSprite.setScale(
      constants::windowWidth / backgroundSprite.getLocalBounds().width,
      constants::windowHeight / backgroundSprite.getLocalBounds().height);
  boardSprite.setScale(
      constants::boardWidth / boardSprite.getLocalBounds().width,
      constants::boardHeight / boardSprite.getLocalBounds().height);

  window->draw(backgroundSprite);
  window->draw(boardSprite);

  // sf::RectangleShape tile(
  //     sf::Vector2f(constants::tileSize, constants::tileSize));

  for (size_t i{0}; i < constants::squares; i++) {
    for (size_t j{0}; j < constants::squares; j++) {
      // if (i == 0 || j == 0)
      //   continue; // Skip labels

      // tile.setFillColor((i + j) % 2 == 0 ? sf::Color::White
      //                                    : sf::Color(150, 75, 0));
      // tile.setPosition(static_cast<float>(j) * constants::tileSize,
      //                  static_cast<float>(i) * constants::tileSize);
      // window->draw(tile);

      // Draw pieces
      int piece = board_[i][j];
      if (piece != 0) {
        sf::Sprite &sprite = pieceSprites[piece];
        sprite.setPosition(static_cast<float>(j) * constants::tileSize +
                               constants::marginSize,
                           static_cast<float>(i) * constants::tileSize +
                               constants::marginSize);
        sprite.setScale(constants::tileSize / sprite.getLocalBounds().width,
                        constants::tileSize / sprite.getLocalBounds().height);
        window->draw(sprite);
      }
    }
  }
}

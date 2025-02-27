#include "graphics.h"
#include "constants.h"
#include <iostream>

BoardGraphics::BoardGraphics(Board &board) : board(board) {
  loadTextures();
  loadPieceTextures();
}

void BoardGraphics::loadTextures() {
  if (!backgroundTexture.loadFromFile("../bases/background.png")) {
    std::cerr << "Failed to load ../bases/background.png" << std::endl;
  }

  if (!boardTexture.loadFromFile("../bases/board.png")) {
    std::cerr << "Failed to load ../bases/board.png" << std::endl;
  }

  backgroundSprite.setTexture(backgroundTexture);
  boardSprite.setTexture(boardTexture);
}

void BoardGraphics::loadPieceTextures() {
  std::map<int, std::string> pieceFiles = {
    {1, "wp.png"},  {2, "wr.png"},  {3, "wn.png"},  {4, "wb.png"},
    {5, "wq.png"},  {6, "wk.png"},  {-1, "bp.png"}, {-2, "br.png"},
    {-3, "bn.png"}, {-4, "bb.png"}, {-5, "bq.png"}, {-6, "bk.png"}
  };

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

void BoardGraphics::draw(sf::RenderWindow &window, bool rotated) {
  // Draw background
  backgroundSprite.setPosition(0, 0);
  backgroundSprite.setScale(
      constants::windowWidth / backgroundSprite.getLocalBounds().width,
      constants::windowHeight / backgroundSprite.getLocalBounds().height);
  window.draw(backgroundSprite);

  // Draw board
  boardSprite.setPosition(constants::marginSize, constants::marginSize);
  boardSprite.setScale(
      constants::boardWidth / boardSprite.getLocalBounds().width,
      constants::boardHeight / boardSprite.getLocalBounds().height);
  window.draw(boardSprite);

  // Draw pieces based on board state
  if (rotated) {
    for (int i = 0; i < constants::squares; i++) {
      for (int j = 0; j < constants::squares; j++) {
        if (board.getPiece(i, j) != nullptr) {
          int pieceType = board.getPiece(i, j)->getType();
          if (pieceSprites.find(pieceType) != pieceSprites.end()) {
            sf::Sprite &sprite = pieceSprites[pieceType];
            sprite.setPosition(static_cast<float>(constants::squares - 1 - i) *
                                   constants::tileSize + constants::marginSize,
                               static_cast<float>(j) * constants::tileSize +
                                   constants::marginSize);
            sprite.setScale(
                constants::tileSize / sprite.getLocalBounds().width,
                constants::tileSize / sprite.getLocalBounds().height);
            window.draw(sprite);
          }
        }
      }
    }
  } else {
    for (int i = 0; i < constants::squares; i++) {
      for (int j = 0; j < constants::squares; j++) {
        if (board.getPiece(i, j) != nullptr) {
          int pieceType = board.getPiece(i, j)->getType();
          if (pieceSprites.find(pieceType) != pieceSprites.end()) {
            sf::Sprite &sprite = pieceSprites[pieceType];
            sprite.setPosition(static_cast<float>(i) * constants::tileSize +
                                   constants::marginSize,
                               static_cast<float>(constants::squares - 1 - j) *
                                   constants::tileSize + constants::marginSize);
            sprite.setScale(
                constants::tileSize / sprite.getLocalBounds().width,
                constants::tileSize / sprite.getLocalBounds().height);
            window.draw(sprite);
          }
        }
      }
    }
  }
}

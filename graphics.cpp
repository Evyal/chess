#include "graphics.h"
#include "constants.h"
#include <iostream>

BoardGraphics::BoardGraphics(Board &board) : board_(board) {
  loadTextures();
  loadPieceTextures();
  initPopupBackground(); // Initialize the popup background here.
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

void BoardGraphics::initPopupBackground() {
  // Set the texture for the popup background using the already loaded
  // backgroundTexture.
  popupBackground.setTexture(backgroundTexture);
  // You can set any default properties here if needed.
  // The texture rectangle and scaling will be updated in showPromotionPopup().
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
        if (board_.getPiece(i, j) != nullptr) {
          int pieceType = board_.getPiece(i, j)->getType();
          if (pieceSprites.find(pieceType) != pieceSprites.end()) {
            sf::Sprite &sprite = pieceSprites[pieceType];
            sprite.setPosition(static_cast<float>(constants::squares - 1 - i) *
                                       constants::tileSize +
                                   constants::marginSize,
                               static_cast<float>(j) * constants::tileSize +
                                   constants::marginSize);
            sprite.setScale(constants::tileSize / sprite.getLocalBounds().width,
                            constants::tileSize /
                                sprite.getLocalBounds().height);
            window.draw(sprite);
          }
        }
      }
    }
  } else {
    for (int i = 0; i < constants::squares; i++) {
      for (int j = 0; j < constants::squares; j++) {
        if (board_.getPiece(i, j) != nullptr) {
          int pieceType = board_.getPiece(i, j)->getType();
          if (pieceSprites.find(pieceType) != pieceSprites.end()) {
            sf::Sprite &sprite = pieceSprites[pieceType];
            sprite.setPosition(static_cast<float>(i) * constants::tileSize +
                                   constants::marginSize,
                               static_cast<float>(constants::squares - 1 - j) *
                                       constants::tileSize +
                                   constants::marginSize);
            sprite.setScale(constants::tileSize / sprite.getLocalBounds().width,
                            constants::tileSize /
                                sprite.getLocalBounds().height);
            window.draw(sprite);
          }
        }
      }
    }
  }
}

int BoardGraphics::showPromotionPopup(bool isWhite) {
  // Define the promotion options.
  // For white: Queen (5), Rook (2), Bishop (4), Knight (3)
  // For black: Queen (-5), Rook (-2), Bishop (-4), Knight (-3)
  int promoOptions[4];
  if (isWhite) {
    promoOptions[0] = 5; // Queen
    promoOptions[1] = 2; // Rook
    promoOptions[2] = 4; // Bishop
    promoOptions[3] = 3; // Knight
  } else {
    promoOptions[0] = -5; // Queen
    promoOptions[1] = -2; // Rook
    promoOptions[2] = -4; // Bishop
    promoOptions[3] = -3; // Knight
  }

  sf::RenderWindow popupWindow(sf::VideoMode(constants::popupPromotionWidth,
                                             constants::popupPromotionHeight),
                               "Pawn Promotion",
                               sf::Style::Titlebar | sf::Style::Close);
  popupWindow.setPosition(
      {constants::windowPosX + constants::windowWidth / 2 +
           constants::marginSize - constants::popupPromotionWidth / 2,
       constants::windowPosY + constants::windowHeight / 2 +
           constants::marginSize - constants::popupPromotionHeight / 2});

  popupBackground.setTextureRect(sf::IntRect(
      0, 0, constants::popupPromotionWidth, constants::popupPromotionHeight));
  // You can scale the background if needed (here it fits exactly).
  popupBackground.setScale(1.0f, 1.0f);

  // Prepare promotion option sprites.
  std::vector<sf::Sprite> promoSprites;
  for (int i = 0; i < 4; i++) {
    // Copy the sprite from our pieceSprites map.
    sf::Sprite sprite = pieceSprites[promoOptions[i]];
    // Scale sprite to exactly fit a tile.
    float scaleX =
        static_cast<float>(constants::tileSize) / sprite.getLocalBounds().width;
    float scaleY = static_cast<float>(constants::tileSize) /
                   sprite.getLocalBounds().height;
    sprite.setScale(scaleX, scaleY);
    // Position each sprite in its slot.
    sprite.setPosition(static_cast<float>(i * constants::tileSize), 0);
    promoSprites.push_back(sprite);
  }

  // Promotion selection loop.
  while (popupWindow.isOpen()) {
    sf::Event event;
    while (popupWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        popupWindow.close();
        // If closed without selection, default to Queen.
        return isWhite ? 5 : -5;
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          int x = event.mouseButton.x;
          int index =
              x / constants::tileSize; // Determine which slot was clicked.
          if (index >= 0 && index < 4) {
            popupWindow.close();
            return promoOptions[index];
          }
        }
      }
    }

    popupWindow.clear();
    // Draw the textured background.
    popupWindow.draw(popupBackground);
    // Then draw the promotion option sprites.
    for (const auto &sprite : promoSprites) {
      popupWindow.draw(sprite);
    }
    popupWindow.display();
  }

  // Fallback: if the loop exits unexpectedly, return Queen.
  return isWhite ? 5 : -5;
}
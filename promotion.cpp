#include "promotion.h"
#include "board.h"
#include <SFML/Window/Event.hpp>

Promotion::Promotion(sf::RenderWindow &window, Board &board, bool isWhite,
                     int x, int y) {
  // Set up window
  window.create(sf::VideoMode(400, 100), "Choose Promotion",
                sf::Style::Titlebar | sf::Style::Close);
  window.setFramerateLimit(60);

  // Get piece types for promotion (Queen, Rook, Bishop, Knight)
  std::vector<int> promotionPieces =
      isWhite ? std::vector<int>{5, 2, 4, 3} : std::vector<int>{-5, -2, -4, -3};

  float spacing = 20.0f;
  float spriteSize = 80.0f;
  float xPos = spacing;

  for (int pieceType : promotionPieces) {
    if (board.pieceSprites.find(pieceType) != board.pieceSprites.end()) {
      sf::Sprite sprite = board.pieceSprites.at(pieceType);
      sprite.setPosition(xPos, 10);
      sprite.setScale(spriteSize / sprite.getLocalBounds().width,
                      spriteSize / sprite.getLocalBounds().height);

      pieceSprites.push_back(sprite);
      pieceTypes.push_back(pieceType);
      xPos += spriteSize + spacing;
    }
  }
}

int PromotionPopup::show() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return pieceTypes[0]; // Default to Queen if closed
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          for (size_t i = 0; i < pieceSprites.size(); ++i) {
            if (pieceSprites[i].getGlobalBounds().contains(
                    event.mouseButton.x, event.mouseButton.y)) {
              window.close();
              return pieceTypes[i];
            }
          }
        }
      }
    }

    window.clear(sf::Color::White);
    for (const auto &sprite : pieceSprites) {
      window.draw(sprite);
    }
    window.display();
  }
  return pieceTypes[0]; // Default to Queen
}

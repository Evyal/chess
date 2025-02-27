#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "board.h"
#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>

class BoardGraphics {
private:
  // Board background and board textures/sprites
  sf::Texture backgroundTexture;
  sf::Texture boardTexture;
  sf::Sprite backgroundSprite;
  sf::Sprite boardSprite;

  // Promotion Popup Background
  sf::Sprite popupBackground;

  // Piece textures and sprites
  std::map<int, sf::Texture> pieceTextures;
  std::map<int, sf::Sprite> pieceSprites;

  // Private helper functions to load images
  void loadTextures();
  void loadPieceTextures();
  void initPopupBackground();

  // Reference to the logic board for accessing piece positions/types
  Board &board_;

public:
  // Constructor takes a reference to a Board instance
  BoardGraphics(Board &board);

  // Draw the board and its pieces on the given window.
  // The 'rotated' flag indicates whether to draw the board rotated.
  void draw(sf::RenderWindow &window, bool rotated);

  // Promotion popup function: opens a window for the user to choose a promotion
  // piece.
  int showPromotionPopup(bool isWhite);
};

#endif // GRAPHICS_H

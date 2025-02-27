#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "board.h"
#include "constants.h"
#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <map>

class BoardGraphics {
private:
  // Board background and board textures/sprites
  sf::Texture backgroundTexture;
  sf::Texture boardTexture;
  sf::Sprite backgroundSprite;
  sf::Sprite boardSprite;

  // Piece textures and sprites
  std::map<int, sf::Texture> pieceTextures;
  std::map<int, sf::Sprite> pieceSprites;

  // Private helper functions to load images
  void loadTextures();
  void loadPieceTextures();

  // Reference to the logic board for accessing piece positions/types
  Board &board;

public:
  // Constructor takes a reference to a Board instance
  BoardGraphics(Board &board);

  // Draw the board and its pieces on the given window.
  // The 'rotated' flag indicates whether to draw the board rotated.
  void draw(sf::RenderWindow &window, bool rotated);
};

#endif // GRAPHICS_H

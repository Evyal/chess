#include "game.h"
#include "board.h"
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game()
    : window(sf::VideoMode(constants::windowWidth, constants::windowHeight),
             "Chess Game"),
      gui(window) {}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::setup() {
  window.setPosition({480, 80});
  window.setFramerateLimit(50);

  createButtons();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      gui.handleEvent(event); // Make sure TGUI processes the event

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    board.draw(window);
    gui.draw(); // Draw GUI elements
    window.display();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::createButtons() {
  for (int i{0}; i < constants::squares; i++) {
    for (int j{0}; j < constants::squares; j++) {
      tiles[i][j] = tgui::Button::create();
      tiles[i][j]->setPosition(i * constants::tileSize + constants::marginSize,
                               j * constants::tileSize + constants::marginSize);
      tiles[i][j]->setSize(constants::tileSize, constants::tileSize);

      // Explicitly set a default color
      tiles[i][j]->getRenderer()->setBackgroundColor(sf::Color::White);
      tiles[i][j]->setInheritedOpacity(0.01f);
      tiles[i][j]->onPress([this, i, j]() { handleButtonClick(i, j); });

      gui.add(tiles[i][j]);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleButtonClick(int row, int col) {

  if (selectedTile.first == row && selectedTile.second == col) {
    return;
  }

  Piece *piece = board.getPiece(col, row);

  // Allow only white pieces to be selected when it's white's turn
  if (piece && (piece->isWhitePiece() == whiteTurn)) {

    if (selectedTile.first != -1 &&
        selectedTile.second != -1) { // Reset the previous selection color
      tiles[selectedTile.first][selectedTile.second]
          ->getRenderer()
          ->setBackgroundColor(sf::Color::White);

      tiles[selectedTile.first][selectedTile.second]
          ->getRenderer()
          ->setBackgroundColorHover(sf::Color::White);

      tiles[selectedTile.first][selectedTile.second]->setInheritedOpacity(
          0.01f);
    }

    // Highlight the new selection
    tiles[row][col]->setInheritedOpacity(0.1f);

    if ((row + col) % 2 == 0) {
      tiles[row][col]->getRenderer()->setBackgroundColorHover(
          sf::Color::Yellow);
      tiles[row][col]->getRenderer()->setBackgroundColor(sf::Color::Yellow);
    } else {
      tiles[row][col]->getRenderer()->setBackgroundColorHover(sf::Color::Red);
      tiles[row][col]->getRenderer()->setBackgroundColor(sf::Color::Red);
    }

    selectedTile = {row, col};

    // Convert row/col to chess notation
    char file = 'a' + static_cast<char>(col); // 'a' to 'h'
    int rank = 8 - row;                       // 8 to 1 (invert row index)

    std::string tileNotation = std::string(1, file) + std::to_string(rank);

    std::cout << "Selected tile: " << tileNotation << '\n';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

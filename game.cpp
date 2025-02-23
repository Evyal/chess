#include "game.h"
#include "board.h"
#include "constants.hpp"
#include "piece.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <fstream>

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
  createTurnLabel();
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
      tiles[i][j]->setPosition((i)*constants::tileSize + constants::marginSize,
                               -(j + 1) * constants::tileSize +
                                   constants::boardHeight +
                                   constants::marginSize);
      tiles[i][j]->setSize(constants::tileSize, constants::tileSize);

      // Explicitly set a default color
      tiles[i][j]->getRenderer()->setBackgroundColor(sf::Color::White);
      tiles[i][j]->setInheritedOpacity(0.01f);
      tiles[i][j]->onPress([this, i, j]() { selectOption(i, j); });

      gui.add(tiles[i][j]);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::selectOption(int row, int col) {

  if (selectedTile.first == -1 && selectedTile.second == -1) {
    selectedTile = {row, col};
  }

  int startX{selectedTile.first};
  int startY{selectedTile.second};
  int endX{row};
  int endY{col};
  Piece *pieceStart = board.getPiece(startX, startY);
  Piece *pieceEnd = board.getPiece(endX, endY);

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // OPTION 1
  if (pieceStart && (pieceStart->isWhitePiece() == whiteTurn)) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (pieceEnd && (pieceEnd->isWhitePiece() == whiteTurn)) {
      tiles[selectedTile.first][selectedTile.second]
          ->getRenderer()
          ->setBackgroundColor(sf::Color::White);

      tiles[selectedTile.first][selectedTile.second]
          ->getRenderer()
          ->setBackgroundColorHover(sf::Color::White);

      tiles[selectedTile.first][selectedTile.second]->setInheritedOpacity(
          0.01f);

      // Highlight the new selection
      tiles[row][col]->setInheritedOpacity(0.1f);

      if ((row + col) % 2 != 0) {
        tiles[row][col]->getRenderer()->setBackgroundColorHover(
            sf::Color::Yellow);
        tiles[row][col]->getRenderer()->setBackgroundColor(sf::Color::Yellow);
      } else {
        tiles[row][col]->getRenderer()->setBackgroundColorHover(sf::Color::Red);
        tiles[row][col]->getRenderer()->setBackgroundColor(sf::Color::Red);
      }

      selectedTile = {row, col};
    } else

      ////////////////////////////////////////////////////////////////////////////////////////////////////

      if (pieceEnd && (pieceEnd->isWhitePiece() != whiteTurn)) {

        tiles[selectedTile.first][selectedTile.second]
            ->getRenderer()
            ->setBackgroundColor(sf::Color::White);

        tiles[selectedTile.first][selectedTile.second]
            ->getRenderer()
            ->setBackgroundColorHover(sf::Color::White);

        tiles[selectedTile.first][selectedTile.second]->setInheritedOpacity(
            0.01f);

        if (pieceStart->isValidMove(startX, startY, endX, endY, board)) {
          board.setPiece(endX, endY, pieceStart);
          board.setPiece(startX, startY, nullptr);
          pieceStart->markAsMoved();

          switchTurn();
        }
      } else

        //////////////////////////////////////////////////////////////////////////////////////////////////////

        if (!pieceEnd) {

          if (pieceStart->getType() == 6 ||
              pieceStart->getType() == -6) { // King
            if (abs(endX - startX) == 2) {
              int rookStartX = (endX > startX) ? 7 : 0;
              int rookEndX = (endX > startX) ? endX - 1 : endX + 1;

              if (pieceStart->hasMovedBefore() == false &&
                  pieceStart->hasMovedBefore() == false) {

                board.setPiece(endX, endY, pieceStart);
                board.setPiece(startX, startY, nullptr);

                Piece *rook = board.getPiece(rookStartX, startY);
                board.setPiece(rookEndX, startY, rook);
                board.setPiece(rookStartX, startY, nullptr);

                pieceStart->markAsMoved();
                rook->markAsMoved();

                tiles[selectedTile.first][selectedTile.second]
                    ->getRenderer()
                    ->setBackgroundColor(sf::Color::White);

                tiles[selectedTile.first][selectedTile.second]
                    ->getRenderer()
                    ->setBackgroundColorHover(sf::Color::White);

                tiles[selectedTile.first][selectedTile.second]
                    ->setInheritedOpacity(0.01f);

                switchTurn();
              }
              return;
            }
          }

          tiles[selectedTile.first][selectedTile.second]
              ->getRenderer()
              ->setBackgroundColor(sf::Color::White);

          tiles[selectedTile.first][selectedTile.second]
              ->getRenderer()
              ->setBackgroundColorHover(sf::Color::White);

          tiles[selectedTile.first][selectedTile.second]->setInheritedOpacity(
              0.01f);

          if (pieceStart->isValidMove(startX, startY, endX, endY, board)) {
            board.setPiece(endX, endY, pieceStart);
            board.setPiece(startX, startY, nullptr);
            pieceStart->markAsMoved();

            switchTurn();
            std::ofstream outputFile("game_moves.txt",
                                     std::ios::app); // Open in append mode
            if (outputFile.is_open()) {
              outputFile << notation(endX, endY) << '\n';
            } else {
              std::cout << "Error opening file for writing.\n";
            }
          }
        }
  } else {
    selectedTile = {-1, -1};
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleMove(int row, int col) {
  if (selectedTile.first == -1 || selectedTile.second == -1) {
    return;
  }

  int startX = selectedTile.first;
  int startY = selectedTile.second;
  int endX = row;
  int endY = col;

  Piece *piece = board.getPiece(startX, startY);
  if (!piece) {
    return;
  }

  if (!piece->isValidMove(startX, startY, endX, endY, board)) {
    std::cout << "Invalid move!\n";
    return;
  }

  Piece *targetPiece = board.getPiece(endX, endY);
  bool capture = targetPiece != nullptr;

  // Move the piece
  board.setPiece(endX, endY, piece);
  board.setPiece(startX, startY, nullptr);

  selectedTile = {-1, -1};
  whiteTurn = !whiteTurn; // Switch turns
  updateTurnLabel();      // Update the display

  // Convert to ASCII notation
  char fileStart = 'a' + static_cast<char>(startY);
  int rankStart = 8 - startX;
  char fileEnd = 'a' + static_cast<char>(endY);
  int rankEnd = 8 - endX;

  std::string moveNotation;

  // Pawn move
  if (piece->getType() == 1 || piece->getType() == -1) { // Pawn
    moveNotation = std::string(1, fileEnd) + std::to_string(rankEnd);
    if (capture) {
      moveNotation = std::string(1, fileStart) + "x" + moveNotation;
    }
  } else {
    // Other piece move
    char pieceChar = piece->getSymbol();
    moveNotation = std::string(1, pieceChar) + std::string(1, fileEnd) +
                   std::to_string(rankEnd);
    if (capture) {
      moveNotation = std::string(1, pieceChar) + "x" + std::string(1, fileEnd) +
                     std::to_string(rankEnd);
    }
  }

  // Output to file
  std::ofstream outputFile("game_moves.txt",
                           std::ios::app); // Open in append mode
  if (outputFile.is_open()) {
    outputFile << moveNotation << '\n';
  } else {
    std::cout << "Error opening file for writing.\n";
  }

  // Also print to console
  std::cout << moveNotation << '\n';
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::switchTurn() {
  selectedTile = {-1, -1};
  whiteTurn = !whiteTurn;
  updateTurnLabel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Game::notation(int row, int col) {
  return std::string(1, 'a' + static_cast<char>(row)) + std::to_string(1 + col);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::createTurnLabel() {
  turnLabel = tgui::Label::create();
  turnLabel->setPosition(constants::turnLabelX, constants::turnLabelY);
  turnLabel->setTextSize(constants::turnLabelSize);
  turnLabel->getRenderer()->setTextColor(sf::Color::White);
  turnLabel->setText("White to move");

  gui.add(turnLabel);
}

void Game::updateTurnLabel() {
  if (whiteTurn) {
    turnLabel->setText("White to move");
    turnLabel->getRenderer()->setTextColor(sf::Color::White);
  } else {
    turnLabel->setText("Black to move");
    turnLabel->getRenderer()->setTextColor(sf::Color::Black);
  }
}
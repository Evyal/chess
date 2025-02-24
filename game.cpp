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

  Move move{selectedTile.first,
            selectedTile.second,
            row,
            col,
            board.getPiece(selectedTile.first, selectedTile.second),
            board.getPiece(row, col)};

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // OPTION 1
  if (move.pieceStart && (move.pieceStart->isWhitePiece() == whiteTurn)) {

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (move.pieceEnd && (move.pieceEnd->isWhitePiece() == whiteTurn)) {
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

      if (move.pieceEnd && (move.pieceEnd->isWhitePiece() != whiteTurn)) {

        tiles[selectedTile.first][selectedTile.second]
            ->getRenderer()
            ->setBackgroundColor(sf::Color::White);

        tiles[selectedTile.first][selectedTile.second]
            ->getRenderer()
            ->setBackgroundColorHover(sf::Color::White);

        tiles[selectedTile.first][selectedTile.second]->setInheritedOpacity(
            0.01f);

        if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                         move.endY, board)) {
          board.setPiece(move.endX, move.endY, move.pieceStart);
          board.setPiece(move.startX, move.startY, nullptr);
          move.pieceStart->markAsMoved();

          switchTurn();
        }
      } else

        //////////////////////////////////////////////////////////////////////////////////////////////////////

        if (!move.pieceEnd) {

          if (move.pieceStart->getType() == 6 ||
              move.pieceStart->getType() == -6) { // King
            if (abs(move.endX - move.startX) == 2) {
              int rookStartX = (move.endX > move.startX) ? 7 : 0;
              int rookEndX =
                  (move.endX > move.startX) ? move.endX - 1 : move.endX + 1;

              if (move.pieceStart->hasMovedBefore() == false &&
                  move.pieceStart->hasMovedBefore() == false) {

                board.setPiece(move.endX, move.endY, move.pieceStart);
                board.setPiece(move.startX, move.startY, nullptr);

                Piece *rook = board.getPiece(rookStartX, move.startY);
                board.setPiece(rookEndX, move.startY, rook);
                board.setPiece(rookStartX, move.startY, nullptr);

                move.pieceStart->markAsMoved();
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

          if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                           move.endY, board)) {
            board.setPiece(move.endX, move.endY, move.pieceStart);
            board.setPiece(move.startX, move.startY, nullptr);
            move.pieceStart->markAsMoved();

            switchTurn();
          }
        }
  } else {
    selectedTile = {-1, -1};
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleMove(int row, int col) {

  Move move{selectedTile.first,
            selectedTile.second,
            row,
            col,
            board.getPiece(selectedTile.first, selectedTile.second),
            board.getPiece(row, col)};

  if (!move.pieceStart) {
    return;
  }

  if (!(move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                     move.endY, board))) {
    std::cout << "Invalid move!\n";
    return;
  }

  Piece *targetPiece = board.getPiece(move.endX, move.endY);
  bool capture = targetPiece != nullptr &&
                 targetPiece->isWhitePiece() != move.pieceStart->isWhitePiece();

  board.setPiece(move.endX, move.endY, move.pieceStart);
  board.setPiece(move.startX, move.startY, nullptr);

  // NOTATION

  switchTurn();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::switchTurn() {
  selectedTile = {-1, -1};
  whiteTurn = !whiteTurn;
  updateTurnLabel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Game::notation(int row, int col) {}

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
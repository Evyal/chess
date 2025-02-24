#include "game.h"
#include "board.h"
#include "constants.h"
#include "piece.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <fstream>
#include <string>

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
      tiles[i][j]->onPress([this, i, j]() { handleButtonClick(i, j); });

      gui.add(tiles[i][j]);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleButtonClick(int row, int col) {

  Piece *piece = board.getPiece(row, col);

  if ((selectedTile.first == -1) && (selectedTile.second == -1)) {
    if (piece && (piece->isWhitePiece() == whiteTurn)) {
      highlightSelection(row, col, true, false);
      selectedTile = {row, col};
    }

    return;
  }

  Move move{selectedTile.first,
            selectedTile.second,
            row,
            col,
            board.getPiece(selectedTile.first, selectedTile.second),
            board.getPiece(row, col)};

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  if (move.pieceEnd && (move.pieceEnd->isWhitePiece() == whiteTurn)) {

    highlightSelection(row, col, true, true);
    selectedTile = {row, col};

  } else

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    if (move.pieceEnd && (move.pieceEnd->isWhitePiece() != whiteTurn)) {

      if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                       move.endY, board)) {
        capturePiece(move);
      }

      // UNSELECT PIECE BY CLICKING AWAY
      else {
        highlightSelection(row, col, false, true);
        selectedTile = {-1, -1};
      }

    } else

      //////////////////////////////////////////////////////////////////////////////////////////////////////

      if (!move.pieceEnd) {
        if (handleCastling(move)) {
          return;
        }

        if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                         move.endY, board)) {
          movePiece(move);
        }

        // UNSELECT PIECE BY CLICKING AWAY
        else {
          highlightSelection(row, col, false, true);
          selectedTile = {-1, -1};
        }

      } else

      //////////////////////////////////////////////////////////////////////////////////////////////////////

      {
        highlightSelection(row, col, false, true);
        selectedTile = {-1, -1};
      }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::highlightSelection(int row, int col, bool highlight,
                              bool unhighlight) {
  if (unhighlight) {
    tiles[selectedTile.first][selectedTile.second]->setInheritedOpacity(0.01f);

    tiles[selectedTile.first][selectedTile.second]
        ->getRenderer()
        ->setBackgroundColor(sf::Color::White);

    tiles[selectedTile.first][selectedTile.second]
        ->getRenderer()
        ->setBackgroundColorHover(sf::Color::White);
  }

  if (highlight) {
    tiles[row][col]->setInheritedOpacity(0.1f);

    if ((row + col) % 2 != 0) {
      tiles[row][col]->getRenderer()->setBackgroundColorHover(
          sf::Color::Yellow);
      tiles[row][col]->getRenderer()->setBackgroundColor(sf::Color::Yellow);
    } else {
      tiles[row][col]->getRenderer()->setBackgroundColorHover(sf::Color::Red);
      tiles[row][col]->getRenderer()->setBackgroundColor(sf::Color::Red);
    }
  }
}

// Moves a piece
void Game::movePiece(const Move &move) {
  board.setPiece(move.endX, move.endY, move.pieceStart);
  board.setPiece(move.startX, move.startY, nullptr);
  move.pieceStart->markAsMoved();

  highlightSelection(move.endX, move.endY, false, true);
  switchTurn();

  logMove(move);
}

// Captures a piece
void Game::capturePiece(const Move &move) {
  board.setPiece(move.endX, move.endY, move.pieceStart);
  board.setPiece(move.startX, move.startY, nullptr);
  move.pieceStart->markAsMoved();

  highlightSelection(move.endX, move.endY, false, true);
  switchTurn();

  logMove(move, true);
}

// Handles castling logic
bool Game::handleCastling(const Move &move) {
  if (move.pieceStart->getType() == 6 ||
      move.pieceStart->getType() == -6) { // King
    if (abs(move.endX - move.startX) == 2) {
      int rookStartX = (move.endX > move.startX) ? 7 : 0;
      int rookEndX = (move.endX > move.startX) ? move.endX - 1 : move.endX + 1;
      Piece *rook = board.getPiece(rookStartX, move.startY);

      board.setPiece(move.endX, move.endY, move.pieceStart);
      board.setPiece(move.startX, move.startY, nullptr);
      board.setPiece(rookEndX, move.startY, rook);
      board.setPiece(rookStartX, move.startY, nullptr);

      move.pieceStart->markAsMoved();
      rook->markAsMoved();

      highlightSelection(move.endX, move.endY, false, true);
      switchTurn();

      logMove(move, false, true); // Log castling
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::switchTurn() {
  selectedTile = {-1, -1};
  whiteTurn = !whiteTurn;
  updateTurnLabel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Game::notation(int row, int col) {
  return std::string(1, 'a' + static_cast<char>(row)) + std::to_string(col + 1);
}

void Game::logMove(const Move &move, bool capture, bool castling) {
  std::ofstream outputFile("game_moves.txt", std::ios::app);
  if (outputFile.is_open()) {
    std::string moveStr{};

    if ((move.pieceStart->getType() == 1) ||
        (move.pieceStart->getType() == -1)) {
      if (capture) {
        moveStr = notation(move.startX, move.startY);
      } else {
        moveStr = notation(move.endX, move.endY);
      }

    } else if (castling) {
      moveStr = std::string("O-O") + (move.endX > move.startX ? "" : "-O");
    } else if (capture) {
      moveStr = move.pieceStart->getSymbol() + static_cast<std::string>("x") +
                notation(move.endX, move.endY);
    } else {
      moveStr = move.pieceStart->getSymbol() + notation(move.endX, move.endY);
    }

    if (!whiteTurn) {
      outputFile << moveStr << ' ';
    } else {
      outputFile << moveStr << '\n';
    }

  } else {
    std::cout << "Error opening file for writing.\n";
  }
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
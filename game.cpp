#include "game.h"
#include "board.h"
#include "constants.h"
#include "move.h"
#include "piece.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
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

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
          undoMove();
        }

        if (event.key.code == sf::Keyboard::Right) {
          redoMove();
        }

        if (event.key.code == sf::Keyboard::R) {
          isRotated = !isRotated;
          rotateBoard();
        }
      }

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    board.draw(window, isRotated);
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
} ////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::rotateBoard() {
  if (isRotated) {
    for (int i{0}; i < constants::squares; i++) {
      for (int j{0}; j < constants::squares; j++) {
        tiles[i][j]->setPosition(
            -(i + 1) * constants::tileSize + constants::boardWidth + constants::marginSize,
            (j)*constants::tileSize + constants::marginSize);
        tiles[i][j]->setSize(constants::tileSize, constants::tileSize);
      }
    }
  } else {
    for (int i{0}; i < constants::squares; i++) {
      for (int j{0}; j < constants::squares; j++) {
        tiles[i][j]->setPosition(
            (i)*constants::tileSize + constants::marginSize,
            -(j + 1) * constants::tileSize + constants::boardHeight +
                constants::marginSize);
        tiles[i][j]->setSize(constants::tileSize, constants::tileSize);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleButtonClick(int row, int col) {

  Piece *piece = board.getPiece(row, col);

  if ((selectedTile.first == -1) && (selectedTile.second == -1)) {
    if (piece && (piece->isWhitePiece() == whiteTurn)) {
      highlightSelection(row, col, true);
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

  assert(move.pieceStart);
  assert(move.pieceStart->isWhitePiece() == whiteTurn);

  if (move.pieceStart->hasMovedBefore()) {
    move.isFirstTimeMoving = false;
  }

  if (move.pieceEnd) {

    if (move.pieceEnd->isWhitePiece() == whiteTurn) {

      highlightSelection(row, col, true);
      selectedTile = {row, col};

      return;

    } else

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    // if (move.pieceEnd->isWhitePiece() != whiteTurn)

    {

      if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                       move.endY, board)) {
        move.isCapture = true;
        handleMove(move);
        selectedTile = {-1, -1};

        return;
      }

      // UNSELECT PIECE BY CLICKING AWAY
      else {
        highlightSelection(row, col, false);
        selectedTile = {-1, -1};

        return;
      }
    }
  } else

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!move.pieceEnd)

    {
      if (move.pieceStart->getType() == 6 || move.pieceStart->getType() == -6) {

        if ((move.endX - move.startX) == 2) {
          move.isShortCastle = true;
          handleMove(move);
          selectedTile = {-1, -1};

          return;
        }

        if ((move.endX - move.startX) == -2) {
          move.isLongCastle = true;
          handleMove(move);
          selectedTile = {-1, -1};

          return;
        }
      }

      if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                       move.endY, board)) {
        move.isMoving = true;
        handleMove(move);
        selectedTile = {-1, -1};

        return;
      }

      // UNSELECT PIECE BY CLICKING AWAY
      else {
        highlightSelection(row, col, false);
        selectedTile = {-1, -1};
      }
    }

    else

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    {
      highlightSelection(row, col, false);
      selectedTile = {-1, -1};
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::highlightSelection(int row, int col, bool highlight) {

  if ((selectedTile.first != -1) && (selectedTile.second != -1)) {
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

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::switchTurn() {
  whiteTurn = !whiteTurn;
  updateTurnLabel();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleMove(const Move &move) {
  if (move.isMoving) {

    board.setPiece(move.endX, move.endY, move.pieceStart);
    board.setPiece(move.startX, move.startY, nullptr);
    move.pieceStart->markAsMoved(true);

  }

  else if (move.isCapture) {

    board.setPiece(move.endX, move.endY, move.pieceStart);
    board.setPiece(move.startX, move.startY, nullptr);
    move.pieceStart->markAsMoved(true);

  }

  else if (move.isShortCastle) {

    int rookStartX = (move.endX > move.startX) ? 7 : 0;
    int rookEndX = (move.endX > move.startX) ? move.endX - 1 : move.endX + 1;
    Piece *rook = board.getPiece(rookStartX, move.startY);

    board.setPiece(move.endX, move.endY, move.pieceStart);
    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(rookEndX, move.startY, rook);
    board.setPiece(rookStartX, move.startY, nullptr);

    move.pieceStart->markAsMoved(true);
    rook->markAsMoved(true);

  }

  else if (move.isLongCastle) {

    int rookStartX = (move.endX > move.startX) ? 7 : 0;
    int rookEndX = (move.endX > move.startX) ? move.endX - 1 : move.endX + 1;
    Piece *rook = board.getPiece(rookStartX, move.startY);

    board.setPiece(move.endX, move.endY, move.pieceStart);
    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(rookEndX, move.startY, rook);
    board.setPiece(rookStartX, move.startY, nullptr);

    move.pieceStart->markAsMoved(true);
    rook->markAsMoved(true);

  }

  else {
    std::cout << "Error!" << '\n';
    return;
  }

  highlightSelection(move.endX, move.endY, false);
  switchTurn();

  moveNumber++;

  if (moveNumber > moveLog.size()) {
    logMove(move);

    assert(moveLog.size() == moveNumber);

  } else if (!(areMovesEqual(move, moveLog.at(moveNumber - 1)))) {
    moveLog.erase(moveLog.begin() + moveNumber - 1, moveLog.end());
    moveLog.emplace_back(move);

    assert(moveLog.size() == moveNumber);
  }

  if (moveNumber % 2 == 0) {
    whiteTurn = true;
  } else {
    whiteTurn = false;
  }

  // logNotation(move);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::logMove(const Move &move) { moveLog.emplace_back(move); }

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::undoMove() {

  if (moveLog.size() == 0) {
    std::cout << "No moves to undo." << std::endl;
    return;
  }

  if (moveNumber == 0) {
    std::cout << "No moves to undo." << std::endl;
    return;
  }

  const Move &move = moveLog.at(moveNumber - 1);

  if (move.isMoving) {

    if (move.pieceEnd) {
      board.setPiece(move.endX, move.endY, move.pieceEnd);
      board.setPiece(move.startX, move.startY, move.pieceStart);
      move.pieceStart->markAsMoved(!move.isFirstTimeMoving);
    } else {
      board.setPiece(move.endX, move.endY, nullptr);
      board.setPiece(move.startX, move.startY, move.pieceStart);
      move.pieceStart->markAsMoved(!move.isFirstTimeMoving);
    }

  }

  else if (move.isCapture) {

    board.setPiece(move.endX, move.endY, move.pieceEnd);
    board.setPiece(move.startX, move.startY, move.pieceStart);
    move.pieceStart->markAsMoved(!move.isFirstTimeMoving);

  }

  else if (move.isShortCastle) {

    int rookStartX = (move.endX > move.startX) ? 7 : 0;
    int rookEndX = (move.endX > move.startX) ? move.endX - 1 : move.endX + 1;
    Piece *rook = board.getPiece(rookStartX, move.startY);

    board.setPiece(move.endX, move.endY, move.pieceStart);
    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(rookEndX, move.startY, rook);
    board.setPiece(rookStartX, move.startY, nullptr);

    move.pieceStart->markAsMoved(false);
    rook->markAsMoved(false);

  }

  else if (move.isLongCastle) {

    int rookStartX = (move.endX > move.startX) ? 7 : 0;
    int rookEndX = (move.endX > move.startX) ? move.endX - 1 : move.endX + 1;
    Piece *rook = board.getPiece(rookStartX, move.startY);

    board.setPiece(move.endX, move.endY, move.pieceStart);
    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(rookEndX, move.startY, rook);
    board.setPiece(rookStartX, move.startY, nullptr);

    move.pieceStart->markAsMoved(false);
    rook->markAsMoved(false);

  }

  else {
    std::cout << "Error!" << '\n';
    return;
  }

  highlightSelection(move.endX, move.endY, false);
  switchTurn();

  moveNumber--;
  // moveLog.pop_back();
  // notationLog.erase(notationLog.end() - 1);

  // assert(moveLog.size() == notationLog.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::redoMove() {

  if (moveNumber >= moveLog.size()) {
    std::cout << "No moves to redo." << std::endl;
    return;
  }

  const Move &move = moveLog.at(moveNumber);

  handleMove(move);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Game::notation(int row, int col) {
  return std::string(1, 'a' + static_cast<char>(row)) + std::to_string(col + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// void Game::logNotation(const Move &move) {
//   std::string moveStr{};

//   if ((move.pieceStart->getType() == 1) || (move.pieceStart->getType() ==
//   -1)) {
//     if (move.isCapture) {
//       moveStr = notation(move.startX, move.startY);
//     } else {
//       moveStr = notation(move.endX, move.endY);
//     }

//   } else if (move.isShortCastle) {
//     moveStr = "O-O";
//   } else if (move.isLongCastle) {
//     moveStr = "O-O-O";
//   } else if (move.isCapture) {
//     moveStr = std::string(1, move.pieceStart->getSymbol()) + 'x' +
//     notation(move.endX, move.endY);

//   } else {
//     moveStr = move.pieceStart->getSymbol() + notation(move.endX, move.endY);
//   }
//   notationLog.emplace_back(move);
// }

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::fileNotation(const Move &move) {
  std::ofstream outputFile("game_moves.txt", std::ios::app);
  if (outputFile.is_open()) {
    std::string moveStr{};

    if ((move.pieceStart->getType() == 1) ||
        (move.pieceStart->getType() == -1)) {
      if (move.isCapture) {
        moveStr = notation(move.startX, move.startY);
      } else {
        moveStr = notation(move.endX, move.endY);
      }

    } else if (move.isShortCastle) {
      moveStr = "O-O";
    } else if (move.isLongCastle) {
      moveStr = "O-O-O";
    } else if (move.isCapture) {
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

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::printMoveParameters() {
  if (moveLog.empty()) {
    std::cout << "No moves to undo." << std::endl;
    return;
  }

  const Move &move = moveLog.back();

  if (move.pieceEnd) {
    std::cout << move.startX << " " << move.startY << " " << move.endX << " "
              << move.endY << " " << move.pieceStart->getSymbol() << " "
              << move.pieceEnd->getSymbol() << '\n';
  } else {
    std::cout << move.startX << " " << move.startY << " " << move.endX << " "
              << move.endY << " " << move.pieceStart->getSymbol() << " "
              << "Empty Square" << '\n';
  }
}

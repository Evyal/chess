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

        if (event.key.code == sf::Keyboard::P) {
          fileNotation();
        }

        if (event.key.code == sf::Keyboard::F) {
          notationFEN();
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
            -(i + 1) * constants::tileSize + constants::boardWidth +
                constants::marginSize,
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
    moveLog.erase(moveLog.begin() + static_cast<long>(moveNumber) - 1,
                  moveLog.end());
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

std::string Game::notationPNG(const Move &move) {
  if (move.pieceStart->getType() == 1 || move.pieceStart->getType() == -1) {
    if (move.isCapture) {
      return std::string(1, 'a' + static_cast<char>(move.startX)) + "x" +
             std::string(1, 'a' + static_cast<char>(move.endX)) +
             std::to_string(move.endY + 1);
    } else {
      return std::string(1, 'a' + static_cast<char>(move.endX)) +
             std::to_string(move.endY + 1);
    }
  }

  if (move.isMoving) {
    return (move.pieceStart->getSymbolPNG()) +
           std::string(1, 'a' + static_cast<char>(move.endX)) +
           std::to_string(move.endY + 1);
  }

  if (move.isShortCastle) {
    return "O-O";
  }
  if (move.isLongCastle) {
    return "O-O-O";
  }
  if (move.isCapture) {
    return (move.pieceStart->getSymbolPNG()) + std::string(1, 'x') +
           std::string(1, 'a' + static_cast<char>(move.endX)) +
           std::to_string(move.endY + 1);
  }
  return "Error!";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::notationFEN() {

  std::ofstream outputFile("positionFEN.txt", std::ios::app);

  if (outputFile.is_open()) {

    std::string str{};
    bool canWhiteCastleK{true};
    bool canWhiteCastleQ{true};
    bool canBlackCastleK{true};
    bool canBlackCastleQ{true};

    for (int j{0}; j < 8; j++) {
      int a{0}; // Track empty squares

      for (int i{0}; i < 8; i++) {

        if (!board.getPiece(i, j)) {
          a++; // Increment 'a' when the square is empty
        } else {
          if (a > 0) {
            str +=
                std::to_string(a); // Add the number of empty squares to 'str'
            a = 0;                 // Reset 'a' after adding it
          }

          str +=
              (board.getPiece(i, j)->getSymbolFEN()); // Add the piece's symbol

          // Handle castling conditions based on piece types and movement
          if (board.getPiece(i, j)->getType() == 6) {
            if (board.getPiece(i, j)->hasMovedBefore()) {
              canWhiteCastleK = canWhiteCastleQ = false;
            }
          }

          if (board.getPiece(i, j)->getType() == -6) {
            if (board.getPiece(i, j)->hasMovedBefore()) {
              canBlackCastleK = canBlackCastleQ = false;
            }
          }

          if (board.getPiece(i, j)->getType() == 2 ||
              board.getPiece(i, j)->getType() == -2) {
            if (board.getPiece(i, j)->hasMovedBefore()) {
              if (board.getPiece(i, j)->getStartingPosition().first <
                  board.getPiece(i, j)->getStartingPosition().first) {
                canWhiteCastleQ = (board.getPiece(i, j)->getType() == 2)
                                      ? false
                                      : canWhiteCastleQ;
                canBlackCastleQ = (board.getPiece(i, j)->getType() == -2)
                                      ? false
                                      : canBlackCastleQ;
              } else {
                canWhiteCastleK = (board.getPiece(i, j)->getType() == 2)
                                      ? false
                                      : canWhiteCastleK;
                canBlackCastleK = (board.getPiece(i, j)->getType() == -2)
                                      ? false
                                      : canBlackCastleK;
              }
            }
          }
        }
      }

      if (a > 0) {
        str += std::to_string(
            a); // Add remaining empty squares if any at the end of the row
      }

      str += "/"; // Add slash at the end of each row
    }

    // Add turn color (white or black)
    str += " ";
    str += (moveNumber % 2 == 0) ? "w" : "b";
    str += " ";

    // Add castling rights
    if (canWhiteCastleK)
      str += "K";
    if (canWhiteCastleQ)
      str += "Q";
    if (canBlackCastleK)
      str += "k";
    if (canBlackCastleQ)
      str += "q";

    outputFile << str << '\n';

  } else {
    std::cout << "Error opening file for writing.\n";
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::fileNotation() {
  std::ofstream outputFile("movesPNG.txt", std::ios::app);
  if (outputFile.is_open()) {

    int a{2};
    for (const auto &i : moveLog) {
      if (a % 2 == 0) {
        outputFile << a / 2 << ". " << notationPNG(i) << " ";
      } else {
        outputFile << notationPNG(i) << '\n';
      }
      a++;
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

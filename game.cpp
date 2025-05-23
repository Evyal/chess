#include "game.h"
#include "board.h"
#include "constants.h"
#include "graphics.h"
#include "move.h"
#include "piece.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

Game::Game()
    : window(sf::VideoMode(constants::windowWidth, constants::windowHeight),
             "Chess Game"),
      boardGraphics{board} {
  gui.setTarget(window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::setup() {
  window.setPosition({constants::windowPosX, constants::windowPosY});
  window.setFramerateLimit(constants::frameRate);

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

        { // ADD CONTROL
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

        if (event.key.code == sf::Keyboard::Left) {
          undoMove();
        }

        if (event.key.code == sf::Keyboard::Right) {
          redoMove();
        }

        if (event.key.code == sf::Keyboard::S) {
          handleCastling(true);
        }

        if (event.key.code == sf::Keyboard::L) {
          handleCastling(false);
        }
      }

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();

    boardGraphics.draw(window, isRotated);
    gui.draw();
    window.display();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
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
            board.getPiece(selectedTile.first, selectedTile.second),
            row,
            col,
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

        if (move.pieceStart->getType() == 1 ||
            move.pieceStart->getType() == -1) { // Pawn
          int promotionRank = move.pieceStart->isWhitePiece() ? 7 : 0;
          if (move.endY == promotionRank) {
            promotePawn(move);
            move.isPromotion = true;
            handleMove(move);
            selectedTile = {-1, -1};

            return;
          }
        }

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

      if (move.pieceStart->isValidMove(move.startX, move.startY, move.endX,
                                       move.endY, board)) {
        move.isMoving = true;

        if (move.pieceStart->getType() == 1 ||
            move.pieceStart->getType() == -1) { // Pawn
          int promotionRank = move.pieceStart->isWhitePiece() ? 7 : 0;
          if (move.endY == promotionRank) {
            promotePawn(move);
            move.isPromotion = true;
            handleMove(move);
            selectedTile = {-1, -1};

            return;
          }
        }

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
////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleMove(const Move &move) {
  if (move.isMoving) {

    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(move.endX, move.endY, move.pieceStart);

    move.pieceStart->markAsMoved(true);

    if (move.isPromotion) {
      board.setPiece(move.endX, move.endY, nullptr);
      board.setPiece(move.endX, move.endY, move.promotionPiece);
    }

  }

  else if (move.isCapture) {

    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(move.endX, move.endY, move.pieceStart);

    move.pieceStart->markAsMoved(true);

    if (move.isPromotion) {
      board.setPiece(move.endX, move.endY, nullptr);
      board.setPiece(move.endX, move.endY, move.promotionPiece);
    }

  }

  else if (move.isShortCastle) {

    Rook *rook = dynamic_cast<Rook *>(move.pieceEnd);

    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(rook->getStartingPosition().first,
                   rook->getStartingPosition().second, nullptr);

    int row = move.pieceStart->isWhitePiece() ? (row = 0) : (row = 7);

    board.setPiece(constants::shortCastleKingX, row, move.pieceStart);
    board.setPiece(constants::shortCastleRookX, row, rook);

    move.pieceStart->markAsMoved(true);
    rook->markAsMoved(true);

  }

  else if (move.isLongCastle) {

    Rook *rook = dynamic_cast<Rook *>(move.pieceEnd);

    board.setPiece(move.startX, move.startY, nullptr);
    board.setPiece(rook->getStartingPosition().first,
                   rook->getStartingPosition().second, nullptr);

    int row = move.pieceStart->isWhitePiece() ? (row = 0) : (row = 7);

    board.setPiece(constants::longCastleKingX, row, move.pieceStart);
    board.setPiece(constants::longCastleRookX, row, rook);

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
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::logMove(const Move &move) { moveLog.emplace_back(move); }

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleCastling(bool kingSide) {
  if ((selectedTile.first == -1) && (selectedTile.second == -1)) {
  } else {
    Move move{selectedTile.first, selectedTile.second,
              board.getPiece(selectedTile.first, selectedTile.second)};

    if (move.pieceStart &&
        (move.pieceStart->getType() == 6 || move.pieceStart->getType() == -6)) {
      King *king = dynamic_cast<King *>(move.pieceStart);

      if (king->canCastle(board, kingSide)) {
        if (kingSide) {
          move.isShortCastle = true;
        } else {
          move.isLongCastle = true;
        }

        Rook *eligibleRook =
            board.getRookForCastling(move.startX, move.startY, kingSide,
                                     move.pieceStart->isWhitePiece());

        move.endX = eligibleRook->getStartingPosition().first;
        move.endY = eligibleRook->getStartingPosition().second;
        move.pieceEnd = eligibleRook;

        handleMove(move);
        selectedTile = {-1, -1};
      } else {
        std::cout << "Cannot Castle! \n";
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::promotePawn(Move &move) {
  // Determine the pawn color 
  bool isWhite = board.getPiece(move.startX, move.startY)->isWhitePiece();

  int chosenPieceType = boardGraphics.showPromotionPopup(isWhite);

  switch (std::abs(chosenPieceType)) {
  case 5: // Queen
    move.promotionPiece = new Queen(isWhite);
    break;
  case 2: // Rook
    move.promotionPiece = new Rook(isWhite);
    break;
  case 4: // Bishop
    move.promotionPiece = new Bishop(isWhite);
    break;
  case 3: // Knight
    move.promotionPiece = new Knight(isWhite);
    break;
  }

  move.promotionPiece->markAsMoved(true);

  // (Optionally update the board graphics if needed.)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
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

    board.setPiece(move.endX, move.endY, nullptr);
    board.setPiece(move.startX, move.startY, move.pieceStart);
    move.pieceStart->markAsMoved(!move.isFirstTimeMoving);

  }

  else if (move.isCapture) {

    board.setPiece(move.endX, move.endY, move.pieceEnd);
    board.setPiece(move.startX, move.startY, move.pieceStart);
    move.pieceStart->markAsMoved(!move.isFirstTimeMoving);

  }

  else if (move.isShortCastle) {

    Rook *rook = dynamic_cast<Rook *>(move.pieceEnd);

    int row{};
    move.pieceStart->isWhitePiece() ? row = 0 : row = 7;

    board.setPiece(constants::shortCastleKingX, row, nullptr);
    board.setPiece(constants::shortCastleRookX, row, nullptr);

    board.setPiece(move.startX, move.startY, move.pieceStart);
    board.setPiece(rook->getStartingPosition().first,
                   rook->getStartingPosition().second, rook);

    move.pieceStart->markAsMoved(false);
    rook->markAsMoved(false);

  }

  else if (move.isLongCastle) {

    Rook *rook = dynamic_cast<Rook *>(move.pieceEnd);

    int row{};
    move.pieceStart->isWhitePiece() ? row = 0 : row = 7;

    board.setPiece(constants::longCastleKingX, row, nullptr);
    board.setPiece(constants::longCastleRookX, row, nullptr);

    board.setPiece(move.startX, move.startY, move.pieceStart);
    board.setPiece(rook->getStartingPosition().first,
                   rook->getStartingPosition().second, rook);

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
  selectedTile = {-1, -1};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::redoMove() {

  if (moveNumber >= moveLog.size()) {
    std::cout << "No moves to redo." << std::endl;
    return;
  }

  const Move &move = moveLog.at(moveNumber);

  handleMove(move);
  selectedTile = {-1, -1};
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Game::notationPGN(const Move &move) {
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
    return (move.pieceStart->getSymbolPGN()) +
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
    return (move.pieceStart->getSymbolPGN()) + std::string(1, 'x') +
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

    for (int j{7}; j >= 0; j--) {
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
  std::ofstream outputFile("movesPGN.txt", std::ios::app);
  if (outputFile.is_open()) {

    int a{2};
    for (const auto &i : moveLog) {
      if (a % 2 == 0) {
        outputFile << a / 2 << ". " << notationPGN(i) << " ";
      } else {
        outputFile << notationPGN(i) << '\n';
      }
      a++;
    }

  } else {
    std::cout << "Error opening file for writing.\n";
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
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

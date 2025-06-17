#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>

class Board;

////////////////////////////////////////////////////////////////////////////////////////////////////
// PIECE CLASS
class Piece {
protected:
  bool isWhite;                             // true = white, false = black
  bool hasMoved_ = false;                   // for castling
  std::pair<int, int> startingPos_{-1, -1}; // for castling

public:
  Piece(bool white);
  Piece(bool white, std::pair<int, int> startingPos);
  virtual ~Piece() = default;

  // GETTERS
  bool isWhitePiece() const { return isWhite; }
  bool hasMovedBefore() const;
  std::pair<int, int> getStartingPosition();

  virtual int getType() = 0;
  virtual char getSymbolPGN() const = 0;
  virtual char getSymbolFEN() const = 0;

  // SETTERS
  void markAsMoved(bool hasMoved);

  // VALID MOVES
  virtual bool isValidMove(int startX, int startY, int endX, int endY,
                           const Board &board) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// PAWN CLASS
class Pawn : public Piece {
public:
  Pawn(bool white);

  int getType() override;
  char getSymbolPGN() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// ROOK CLASS
class Rook : public Piece {

public:
  // Need starting position for rook (chess960), so overloaded constructor
  Rook(bool white);
  Rook(bool white, std::pair<int, int> startingPos);

  int getType() override;
  char getSymbolPGN() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// KNIGHT CLASS
class Knight : public Piece {
public:
  Knight(bool white);

  int getType() override;
  char getSymbolPGN() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// BISHOP CLASS
class Bishop : public Piece {
public:
  Bishop(bool white);

  int getType() override;
  char getSymbolPGN() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// QUEEN CLASS

class Queen : public Piece {
public:
  Queen(bool white);

  int getType() override;
  char getSymbolPGN() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// KING CLASS
class King : public Piece {
public:
  // Need starting position for king (chess960), so overloaded constructor
  King(bool white);
  King(bool white, std::pair<int, int> startingPos);

  int getType() override;
  char getSymbolPGN() const override;
  virtual char getSymbolFEN() const override;

  // VALID MOVES, additional function to check CASTLING
  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
  bool canCastle(const Board &board, bool kingSide);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

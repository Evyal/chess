#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <utility>

class Board;
class Piece {
protected:
  bool isWhite; // true = white, false = black
  bool hasMoved_ = false;
  std::pair<int, int> startingPos_{-1, -1};

public:
  Piece(bool white);
  Piece(bool white, std::pair<int, int> startingPos);
  virtual ~Piece() = default;

  bool isWhitePiece() const { return isWhite; }
  virtual int getType() = 0;
  virtual char getSymbolPNG() const = 0;
  virtual char getSymbolFEN() const = 0;

  void markAsMoved(bool hasMoved);
  bool hasMovedBefore() const;
  std::pair<int, int> getStartingPosition();

  virtual bool isValidMove(int startX, int startY, int endX, int endY,
                           const Board &board) = 0;
};

class Pawn : public Piece {
public:
  Pawn(bool white);

  int getType() override;
  char getSymbolPNG() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Rook : public Piece {

public:
  Rook(bool white);
  Rook(bool white, std::pair<int, int> startingPos);

  int getType() override;
  char getSymbolPNG() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Knight : public Piece {
public:
  Knight(bool white);

  int getType() override;
  char getSymbolPNG() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Bishop : public Piece {
public:
  Bishop(bool white);

  int getType() override;
  char getSymbolPNG() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Queen : public Piece {
public:
  Queen(bool white);

  int getType() override;
  char getSymbolPNG() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class King : public Piece {
public:
  King(bool white);
  King(bool white, std::pair<int, int> startingPos);

  int getType() override;
  char getSymbolPNG() const override;
  virtual char getSymbolFEN() const override;

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
  bool canCastle(const Board &board, bool kingSide);
};

#endif

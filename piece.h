#ifndef PIECE_H
#define PIECE_H

class Board;

class Piece {
protected:
  bool isWhite; // true = white, false = black
  bool hasMoved = false;

public:
  Piece(bool white) : isWhite(white) {}
  virtual ~Piece() = default;

  virtual int getType() = 0;
  virtual char getSymbol() const = 0;
  bool isWhitePiece() const { return isWhite; }

  virtual bool isValidMove(int startX, int startY, int endX, int endY,
                           const Board &board) = 0;

  void markAsMoved() { hasMoved = true; }
  bool hasMovedBefore() const { return hasMoved; }
};

class Pawn : public Piece {
public:
  Pawn(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'P'; }

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Rook : public Piece {
public:
  Rook(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'R'; }

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Knight : public Piece {
public:
  Knight(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'N'; }

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Bishop : public Piece {
public:
  Bishop(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'B'; }

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class Queen : public Piece {
public:
  Queen(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'Q'; }

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

class King : public Piece {
public:
  King(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'K'; }

  bool isValidMove(int startX, int startY, int endX, int endY,
                   const Board &board) override;
};

#endif

#ifndef PIECE_H
#define PIECE_H

class Piece {
protected:
  bool isWhite; // true = white, false = black

public:
  Piece(bool white) : isWhite(white) {}
  virtual ~Piece() = default;

  virtual int getType() = 0;
  virtual char getSymbol() const = 0;
  bool isWhitePiece() const { return isWhite; }

  virtual bool isValidMove(int startX, int startY, int endX, int endY) = 0;
};

class Pawn : public Piece {
public:
  Pawn(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'P'; }

  bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Rook : public Piece {
public:
  Rook(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'R'; }

  bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Knight : public Piece {
public:
  Knight(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'N'; }

  bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Bishop : public Piece {
public:
  Bishop(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'B'; }

  bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Queen : public Piece {
public:
  Queen(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'Q'; }

  bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class King : public Piece {
public:
  King(bool white) : Piece(white) {}

  int getType() override;
  char getSymbol() const override { return 'K'; }

  bool isValidMove(int startX, int startY, int endX, int endY) override;
};

#endif

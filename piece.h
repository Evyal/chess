#ifndef PIECE_H
#define PIECE_H

class Piece {
public:
    virtual bool isValidMove(int startX, int startY, int endX, int endY) = 0;
    virtual char getSymbol() const = 0; // For displaying the piece
    // You might also want to add color or other attributes
};

class Pawn : public Piece {
public:
    bool isValidMove(int startX, int startY, int endX, int endY) override;
    char getSymbol() const override { return 'P'; }
};

class Rook : public Piece {
public:
    bool isValidMove(int startX, int startY, int endX, int endY) override;
    char getSymbol() const override { return 'R'; }
};

class Bishop : public Piece {
public:
    bool isValidMove(int startX, int startY, int endX, int endY) override;
    char getSymbol() const override { return 'B'; }
};

class Knight : public Piece {
public:
    bool isValidMove(int startX, int startY, int endX, int endY) override;
    char getSymbol() const override { return 'N'; }
};

class Queen : public Piece {
public:
    bool isValidMove(int startX, int startY, int endX, int endY) override;
    char getSymbol() const override { return 'Q'; }
};

#endif

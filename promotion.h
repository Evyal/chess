#ifndef PROMOTION_H
#define PROMOTION_H

#include <SFML/Graphics.hpp>
#include "board.h"

class Promotion {
private:
    sf::RenderWindow& window;
    sf::RectangleShape popup;
    std::map<int, sf::Sprite> promotionSprites;
    int selectedPiece;

public:
    Promotion(sf::RenderWindow& window, Board& board, bool isWhite, int x, int y);
    int getSelectedPiece() const;
    void handleEvent(const sf::Event& event);
    void draw();
};

#endif
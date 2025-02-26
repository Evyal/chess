#ifndef RANDOM_H
#define RANDOM_H

#include "board.h"
#include "constants.h"
#include "piece.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////////////////
// Random for generating NUMBERS

int randomInt(int min, int max);
float randomFloat(float min, float max);

//////////////////////////////////////////////////////////////////////////////////////////
// Random for generating PIECES POSITION for CHESS 960

// size_t randomPiecePosition();

#endif
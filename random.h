#ifndef RANDOM_H
#define RANDOM_H

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include "board.h"
#include "constants.h"
#include "piece.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Random for generating NUMBERS

size_t randomInt(size_t min, size_t max);
float randomFloat(float min, float max);

//////////////////////////////////////////////////////////////////////////////////////////
// Random for generating PIECES POSITION for CHESS 960

Position randomPiecePosition();

#endif
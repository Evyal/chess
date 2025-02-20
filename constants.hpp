#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

namespace constants {
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Window constanst
inline constexpr int squares{8};
inline constexpr int tileSize{90};
inline constexpr int tileNumber{squares * squares};

inline constexpr int boardWidth{static_cast<int>(tileSize) * squares};
inline constexpr int boardHeight{static_cast<int>(tileSize) * squares};
inline constexpr int marginSize{40};

inline constexpr int windowWidth{boardWidth + 2 * marginSize};
inline constexpr int windowHeight{boardHeight + 2 * marginSize};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace constants
#endif
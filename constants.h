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

inline constexpr int windowPosX{480};
inline constexpr int windowPosY{80};
inline constexpr int frameRate{50};

inline constexpr int popupPromotionWidth{tileSize * 4};
inline constexpr int popupPromotionHeight{tileSize};

inline constexpr int turnLabelX{300};
inline constexpr int turnLabelY{3};
inline constexpr int turnLabelSize{30};

inline constexpr int shortCastleKingX{6};
inline constexpr int shortCastleRookX{5};
inline constexpr int longCastleKingX{2};
inline constexpr int longCastleRookX{3};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace constants
#endif
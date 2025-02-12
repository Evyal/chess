#include "board.h"
#include "constants.hpp"
#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(
      sf::VideoMode(constants::windowWidth, constants::windowHeight),
      "Chess Board");
  window.setPosition({480, 80});
  Board board(&window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear();
    board.draw();
    window.display();
  }

  return 0;
}

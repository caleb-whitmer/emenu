#include <SFML/Graphics.hpp>

import std;
import UserInputState;

int main(int argc, char const *argv[]) {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Main Window");
  UserInputState currUserInput;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      // If the window close option is selected
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::TextEntered>()) {
        currUserInput.addChar(event->getIf<sf::Event::TextEntered>()->unicode);
        currUserInput.print();
      }
      if (event->is<sf::Event::KeyPressed>()) {
        if (currUserInput.addControl(event->getIf<sf::Event::KeyPressed>()->code))
          currUserInput.print();
      }
    }

    window.clear(sf::Color::White);
    window.display();
  }
  return 0;
}
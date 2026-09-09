#include <SFML/Graphics.hpp>

import std;
import UserInputState;
import UserInterface;

int main(int argc, char const *argv[]) {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Main Window");
  UserInputState inputState;
  UserInterface ui(window);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      // If the window close option is selected
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::TextEntered>()) {
        inputState.addChar(event->getIf<sf::Event::TextEntered>()->unicode);
        // inputState.print();
        ui.updateText(inputState.getText());
      }
      if (event->is<sf::Event::KeyPressed>()) {
        if (inputState.addControl(event->getIf<sf::Event::KeyPressed>()->code)) {
          // inputState.print();
          ui.updateText(inputState.getText());
        }
      }
    }
  }
  return 0;
}
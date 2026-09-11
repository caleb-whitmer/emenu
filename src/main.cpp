#include <SFML/Graphics.hpp>

import std;
import SearchBar;

int main(int argc, char const *argv[]) {

  sf::RenderWindow window(sf::VideoMode({800, 600}), "Main Window", sf::Style::None);
  SearchBar search(window);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      // If the window close option is selected
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::TextEntered>()) {
        search.input(event->getIf<sf::Event::TextEntered>()->unicode);
        search.update();
      }
      if (event->is<sf::Event::KeyPressed>()) {
        search.input(event->getIf<sf::Event::KeyPressed>()->code);
        search.update();
      }
    }
  }
  return 0;
}
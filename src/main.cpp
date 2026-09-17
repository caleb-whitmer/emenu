#include <SFML/Graphics.hpp>

import std;
import SearchBox;
import Settings;

/**
 * @brief      Redraw the given contents on the given window
 *
 * @param      window   The window
 * @param      objects  The contents to be drawn
 * @param[in]  color    The background color
 */
void redraw(  sf::RenderWindow& window, 
              std::vector<std::reference_wrapper<const sf::Drawable>>&& objects,
              sf::Color color = sf::Color::White ) {
  window.clear(color);
  for (const auto& obj : objects)
    window.draw(obj);
  window.display();
}

int main(int argc, char const *argv[]) {
  /**
   * KNOWN BUGS:
   * - Pressing two keys at the exact same time causes it to crash
   */


  std::vector<sf::String> text{
    "hello",
    "woman",
    "world",
    "this",
    "man",
    "ghost",
    "is",
    "e",
    "a",
    "test",
    "mansion",
    "demon",
    "ghoul",
    "space"
  };

  sf::Font font("/usr/share/fonts/gnu-free/FreeSans.otf");

  Settings settings(font);

  SearchBox entry(settings, std::move(text));


  sf::RenderWindow window(
    sf::VideoMode(static_cast<sf::Vector2u>(entry.getSize())), 
    "emenu", 
    sf::Style::None );


  redraw(window, {entry});

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      // If the window close option is selected
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      if (event->is<sf::Event::TextEntered>()) {
        entry.input(event->getIf<sf::Event::TextEntered>()->unicode);
        redraw(window, {entry});
      }
      if (event->is<sf::Event::KeyPressed>()) {
        entry.control(event->getIf<sf::Event::KeyPressed>()->code);
        redraw(window, {entry});
      }
    }
  }
  return 0;
}
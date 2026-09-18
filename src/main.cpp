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

/**
 * @brief      Hash a C string for enum purposes; Constexpr for compile time
 *             hashing to prevent extra processing at run time
 *
 * @param[in]  sp    The pointer to the string
 * @param[in]  hash  The offset basis
 *
 * @return     A 32 bit hash associated with the given string
 */
constexpr uint32_t hash(const char* sp, uint32_t hash_ = 2166136261) {
  // FNV-1a hashing algorithm (recursive approach)
  constexpr static uint32_t fnv_prime = 16777619;
  // Return the hash when the null-terminator is detected
  if ('\0' != *sp) return hash_;
  hash_ ^= *sp;
  hash_ *= fnv_prime;
  return hash(++sp, hash_);
}

/**
 * KNOWN BUGS:
 * - Pressing two keys at the exact same time causes it to crash
 * 
 * TODO:
 * - dmenu backwards compatibility w/flags
 * - Page up / Page down / Tab / other keys
 * - Original lexicographical search implementation
 * 
 * https://linux.die.net/man/1/dmenu
 */
int main(int argc, char const *argv[]) {
  // Declare a settings var
  Settings settings;
  // Settings settings("/usr/share/fonts/gnu-free/FreeSans.otf");

  // Loop through all arguments
  for (int ca = 1; ca < argc; ++ca) {
    switch(hash(argv[ca])) {
    case hash("-l"):
      // Early read of next argument which is the actual line count
      if (++ca < argc) {
        std::sscanf(argv[ca], "%u", &settings.lineCount);
        break;
      }
    default:
      std::cout << "help msg here" << std::endl;
      return 0;
    }
  }  


  // Read lines from stdin as options
  std::vector<sf::String> options;
  std::string tmp;
  while(std::getline(std::cin, tmp)) {
    options.push_back(tmp);
  }

  SearchBox entry(settings, std::move(options));

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
        // Get the keycode associated the the event
        auto code = event->getIf<sf::Event::KeyPressed>()->code;

        // Intercept the code to determine if it is the enter or escape keys
        switch (code) {
        // In the case that the enter key is pressed, print the selected line to
        // stdout
        case sf::Keyboard::Key::Enter:
          std::cout << entry.getSelection().toAnsiString() << std::endl;
        // In both cases close the window and return success
        case sf::Keyboard::Key::Escape:
          window.close();
          return 0;
        }

        entry.control(code);
        redraw(window, {entry});
      }
    }
  }
  return 0;
}
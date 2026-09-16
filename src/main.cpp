#include <SFML/Graphics.hpp>

import std;
// import SearchBox;
import FuzzySort;

int main(int argc, char const *argv[]) {
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
  std::sort(text.begin(), text.end(), fuzzyCmp("MANS", FuzzySort::Insensitive));
  for (const auto& str : text) {
    std::cout << str.toAnsiString() << std::endl;
  }
  return 0;
}

// int main(int argc, char const *argv[]) {
//   /**
//    * KNOWN BUGS:
//    * - Pressing two keys at the exact same time causes it to crash
//    */

//   sf::RenderWindow window(sf::VideoMode({800, 600}), "Main Window", sf::Style::None);
//   sf::Font font("/usr/share/fonts/gnu-free/FreeSans.otf");

//   SearchBox entry(  font, 
//                     20/*font size*/, 
//                     600/*width*/, 
//                     {5, 5}/*padding*/, 
//                     sf::Color::Black/*forground color*/, 
//                     sf::Color::Cyan/*background color*/, 
//                     ""/*starting text*/,
//                     {"a", "b", "c"}/*lines*/,
//                     10/*line count*/  );
//   entry.setPosition({40, 40});
//   window.clear(sf::Color::White);
//   window.draw(entry);
//   window.display();

//   while (window.isOpen()) {
//     while (const std::optional event = window.pollEvent()) {
//       // If the window close option is selected
//       if (event->is<sf::Event::Closed>()) {
//         window.close();
//       }
//       if (event->is<sf::Event::TextEntered>()) {
//         entry.input(event->getIf<sf::Event::TextEntered>()->unicode);
//         // entry.update();
//         window.clear(sf::Color::White);
//         window.draw(entry);
//         window.display();
//       }
//       if (event->is<sf::Event::KeyPressed>()) {
//         entry.control(event->getIf<sf::Event::KeyPressed>()->code);
//         // entry.update();
//         window.clear(sf::Color::White);
//         window.draw(entry);
//         window.display();
//       }
//     }
//   }
//   return 0;
// }
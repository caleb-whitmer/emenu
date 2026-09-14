module;
#include <SFML/Graphics.hpp>
export module SearchBox;
import EntryBox;
import TextBox;
import std;

export class SearchBox final : public EntryBox {
 public:
  // using EntryBox::EntryBox;
  SearchBox(  const sf::Font& font,
              unsigned fontSize, 
              unsigned width,
              sf::Vector2f padding,
              sf::Color fgColor,
              sf::Color bgColor,
              sf::String text,
              const std::vector<sf::String>& lines,
              unsigned lineCount ) 
  : EntryBox{font, fontSize, width, padding, fgColor, bgColor, text},
    _lines{lines},
    _lineCount{lineCount} {}
 private:
  const std::vector<sf::String>& _lines;
  unsigned _lineCount;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    EntryBox::draw(target, states);
  }

  void onTextUpdate() override {
    std::cout << "Updated!" << std::endl;

    /**
     * Fuzzy search stuff here
     */
  }
};
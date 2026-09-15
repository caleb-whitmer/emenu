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
              std::vector<sf::String>&& lines,
              unsigned lineCount ) 
  : EntryBox{font, fontSize, width, padding, fgColor, bgColor, text},
    _lines{std::move(lines)},
    _lineCount{lineCount},
    _recBox{font, fontSize, width, padding, fgColor, sf::Color::Red, ""} {}
 private:
  std::vector<sf::String> _lines;
  unsigned _lineCount;
  TextBox _recBox;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    static float height = this->getSize().y;

    // Make a copy of the recommendation box to be edited and duplicated  
    static TextBox recBox = _recBox;

    // Draw the entry box
    EntryBox::draw(target, states);

    // Offset the states transform by the transform of the search box
    states.transform = this->getTransform() * states.transform;


    unsigned i = 0;
    for (const auto& line : _lines) {
      recBox.setString(line);
      states.transform.translate({0, height});
      target.draw(recBox, states);

      // If we have reached the desired line count then break
      if (++i >= _lineCount) break;
    }
  }

  void onTextUpdate() override {
    /**
     * Fuzzy search stuff here
     */
  }
};
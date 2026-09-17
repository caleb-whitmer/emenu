module;
#include <SFML/Graphics.hpp>
export module SearchBox;
import EntryBox;
import TextBox;
import FuzzySort;
import Settings;
import std;

export class SearchBox final : public EntryBox {
 public:
  SearchBox(const Settings& s, std::vector<sf::String>&& lines)
  : EntryBox{ s.font, 
              s.fontSize, 
              s.width, 
              s.padding, 
              s.foreground, 
              s.background, 
              s.prompt },
    _foreground{s.foreground},
    _background{s.background},
    _foregroundSelected{s.foregroundSelected},
    _backgroundSelected{s.backgroundSelected},
    _lineCount{s.lineCount},
    _recBox{  s.font, 
              s.fontSize, 
              s.width, 
              s.padding, 
              s.foreground, 
              s.background,
              s.prompt,
              s.promptColor  },
    _lines{std::move(lines)},
    _lineSelected{_lines.cbegin()} {}

  /**
   * @brief      Gets the total size of the search box
   *
   * @return     The size of the search box along with the displayed options
   */
  inline sf::Vector2f getSize() {
    return EntryBox::getSize()
            .componentWiseMul({1, static_cast<float>(_lineCount)});
  }
 private:
  sf::Color _foreground;
  sf::Color _background;
  sf::Color _foregroundSelected;
  sf::Color _backgroundSelected;
  TextBox _recBox;
  unsigned _lineCount;
  std::vector<sf::String> _lines;
  std::vector<sf::String>::const_iterator _lineSelected;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    static float height = EntryBox::getSize().y;

    // Make a copy of the recommendation box to be edited and duplicated  
    static TextBox recBox = _recBox;

    // Draw the entry box
    EntryBox::draw(target, states);

    // Offset the states transform by the transform of the search box
    states.transform = this->getTransform() * states.transform;


    unsigned i = 0;
    for (auto it = _lines.cbegin(); it != _lines.cend(); ++it) {
      // Set the text of the current recommendation box to that of its
      // corresponding line entry
      recBox.setString(*it);

      // Position the current Recommendation box to be underneath the previous
      states.transform.translate({0, height});

      // If the current line is selected that color accordingly
      if (it == _lineSelected) {
        recBox.setBackgroundColor(_backgroundSelected);
        recBox.setForgroundColor(_foregroundSelected);
      }

      // Draw the current recommendation box with the calculated transform 
      target.draw(recBox, states);

      // Reset the colors
      recBox.setBackgroundColor(_background);
      recBox.setForgroundColor(_foreground);

      // If we have reached the desired line count then break
      if (++i >= _lineCount) break;
    }
  }

  /**
   * @brief      Called each time the text within the search box is updated
   */
  void onTextUpdate() override {
    // Sort the text based on the search pattern
    std::sort(  _lines.begin(), 
                _lines.end(), 
                fuzzyCmp(this->getString(), Case::Insensitive)  );

    // Reset the selecte line
    _lineSelected = _lines.cbegin();
  }
};
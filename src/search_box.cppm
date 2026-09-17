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
    _lineSelected{} {
    // Cache the size of the lines vector as it wont be changed ever
    _lineCap = _lines.size();
  }

  inline const sf::String& getSelection() const {
    return _lines[_lineSelected];
  }  

  /**
   * @brief      Gets the total size of the search box
   *
   * @return     The size of the search box along with the displayed options
   */
  inline sf::Vector2f getSize() const {
    return EntryBox::getSize()
            .componentWiseMul({1, static_cast<float>(1 + _lineCount)});
  }

  /**
   * @brief      Intercept the control key detection to test for up/down arrow
   *             keys. If the desired keys are detected then use them to control
   *             which line is selected.
   *
   * @param[in]  k     The control key being pressed
   */
  void control(sf::Keyboard::Key k) {
    // Intercept Up and Down arrow keys
    switch (k) {
    case sf::Keyboard::Key::Up:
      // Do not set the index to a negative number
      if(_lineSelected) --_lineSelected;
      return;
    case sf::Keyboard::Key::Down:
      // Prevent the index from exceeding the limits of the vector
      if(_lineSelected < _lineCap-1) ++_lineSelected;
      return;
    }

    // If the key is not handled then pass it along to the parent class
    EntryBox::control(k);
  }
 private:
  sf::Color _foreground;
  sf::Color _background;
  sf::Color _foregroundSelected;
  sf::Color _backgroundSelected;
  TextBox _recBox;
  unsigned _lineCount;
  unsigned _lineCap;
  std::vector<sf::String> _lines;
  unsigned _lineSelected;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    static float height = EntryBox::getSize().y;

    // Make a copy of the recommendation box to be edited and duplicated  
    static TextBox recBox = _recBox;

    // Draw the entry box
    EntryBox::draw(target, states);

    // Offset the states transform by the transform of the search box
    states.transform = this->getTransform() * states.transform;

    // Calculate the starting position to display the lines based on the current
    // location of the selected line
    unsigned localStart = (_lineSelected / _lineCount) * _lineCount;
    // Calculate an upper limit to the draw loops the minimum of the capacity of
    // the lines and the local starting point off set by the desired line count
    // to display
    unsigned limit = std::min(_lineCap, _lineCount + localStart);
    // Loop from the local starting point to which ever limit is found
    for (unsigned i = localStart; i < limit; ++i) {
      // Set the text of the current recommendation box to that of its
      // corresponding line entry
      recBox.setString(_lines[i]);

      // Position the current Recommendation box to be underneath the previous
      states.transform.translate({0, height});

      // If the current line is selected that color accordingly
      if (i == _lineSelected) {
        recBox.setBackgroundColor(_backgroundSelected);
        recBox.setForgroundColor(_foregroundSelected);
      }

      // Draw the current recommendation box with the calculated transform 
      target.draw(recBox, states);

      // Reset the colors
      recBox.setBackgroundColor(_background);
      recBox.setForgroundColor(_foreground);
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

    // Reset the selected line
    _lineSelected = 0;
  }
};
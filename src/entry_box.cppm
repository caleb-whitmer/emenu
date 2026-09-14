module;
#include <SFML/Graphics.hpp>
export module EntryBox;
import std;
import UserInputState;
import TextBox;

#define CURSOR_WIDTH 2

export class EntryBox : public TextBox {
 public: 
  EntryBox( const sf::Font& font,
            unsigned fontSize, 
            unsigned width,
            sf::Vector2f padding,
            sf::Color fgColor,
            sf::Color bgColor,
            sf::String text ) 
  : TextBox{font, fontSize, width, padding, fgColor, bgColor, text},
    _searchCursor{{CURSOR_WIDTH, static_cast<float>(fontSize)}} {

    _searchCursor.setFillColor(fgColor);
  }

  /**
   * @brief      Input a character to the entry box
   *
   * @param[in]  c     The character to be inputted
   */
  void input(char32_t c) {
    _state.addChar(c);
    _updateEntry();

    onTextUpdate();
  }

  /**
   * @brief      Input a control key to the entry box (i.e. arrow keys)
   *
   * @param[in]  k     The key to be inputted
   */
  void control(sf::Keyboard::Key k) {
    if (_state.addControl(k))
      _updateEntry();
  }

 protected:
  virtual void onTextUpdate() {}

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    // Draw the textbox
    TextBox::draw(target, states);

    // Align the cursor and draw it as well
    states.transform = this->getTransform() * states.transform;
    states.transform.translate(this->padding);
    target.draw(_searchCursor, states);
  }

 private:
  UserInputState _state;
  sf::RectangleShape _searchCursor;
  
  /**
   * @brief      Update the text stored in the entry box
   */
  void _updateEntry() {
    // Update the search text to reflect the new input
    this->setString(_state.getText());

    // Get the index of the glyph representing the character currently selected
    // by the cursor
    std::size_t glyphIndex = _state.getCursorIndex();

    // If we are at the zero-th position then skip cursor positioning
    // calculations
    if (!glyphIndex) {
      _searchCursor.setPosition(this->foreground.getPosition());
      // update();
      return;
    }

    // Otherwise place the position of the cursor at the end of the glyph it is
    // selecting
    auto currGlyph = this->foreground.getShapedGlyphs()[glyphIndex - 1];
    float cursorPos = 
      this->foreground.getPosition().x + 
      currGlyph.position.x +
      currGlyph.glyph.bounds.size.x;
    _searchCursor.setPosition({cursorPos, this->foreground.getPosition().y});
  }
};

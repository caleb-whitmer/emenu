module;
#include <SFML/Graphics.hpp>
export module SearchBar;
import std;
import UserInputState;

#define FONT_SIZE 20
#define CURSOR_WIDTH 2
#define SEARCH_TEXT_POS {40, 40}
#define SEARCH_TEXT_COLOR sf::Color::Black

export class SearchBar final {
 public:
  SearchBar(sf::RenderWindow& window)
  : _window{&window}, 
    _font{"/usr/share/fonts/gnu-free/FreeSans.otf"},
    _searchText{_font, "", FONT_SIZE},
    _searchCursor{{CURSOR_WIDTH, FONT_SIZE}} {

      _searchText.setPosition(SEARCH_TEXT_POS);
      _searchText.setFillColor(SEARCH_TEXT_COLOR);

      _searchCursor.setPosition(SEARCH_TEXT_POS);
      _searchCursor.setFillColor(SEARCH_TEXT_COLOR);

    // Immediately update the window once to prevent visual glitches
    update();
  }

  /**
   * @brief      Input a character to the search bar
   *
   * @param[in]  c     The character to be inputted
   */
  void input(char32_t c) {
    _state.addChar(c);
    _updateText();
  }

  /**
   * @brief      Input a control key to the search bar (i.e. arrow keys)
   *
   * @param[in]  k     The key to be inputted
   */
  void input(sf::Keyboard::Key k) {
    if (_state.addControl(k))
      _updateText();
  }

  /**
   * @brief      Update the user interface
   */
  void update() {
    _window->clear(sf::Color::White);

    _window->draw(_searchText);
    _window->draw(_searchCursor);

    _window->display();
  }
 private:
  
  /**
   * @brief      Update the text stored in the search bar
   */
  void _updateText() {
    // Update the search text to reflect the new input
    _searchText.setString(_state.getText());

    // Get the index of the glyph representing the character currently selected
    // by the cursor
    std::size_t glyphIndex = _state.getCursorIndex();

    // If we are at the zero-th position then skip cursor positioning
    // calculations
    if (!glyphIndex) {
      _searchCursor.setPosition(_searchText.getPosition());
      update();
      return;
    }

    // Otherwise place the position of the cursor at the end of the glyph it is
    // selecting
    auto currGlyph = _searchText.getShapedGlyphs()[glyphIndex - 1];
    float cursorPos = 
      _searchText.getPosition().x + 
      currGlyph.position.x +
      currGlyph.glyph.bounds.size.x;
    _searchCursor.setPosition({cursorPos, _searchText.getPosition().y});
  }

  UserInputState _state;
  sf::RenderWindow* _window;
  const sf::Font _font;
  sf::Text _searchText;
  sf::RectangleShape _searchCursor;
};
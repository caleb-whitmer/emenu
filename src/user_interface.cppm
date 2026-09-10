module;
#include <SFML/Graphics.hpp>
export module UserInterface;
import std;
import UserInputState;

#define FONT_SIZE 20
#define CURSOR_WIDTH 2
#define SEARCH_TEXT_POS {40, 40}
#define SEARCH_TEXT_COLOR sf::Color::Black

export class UserInterface final {
 public:
  UserInterface(sf::RenderWindow& window)
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
   * @brief      Update the visuals of the search bar
   *
   * @param[in]  state  The state of the user input text
   */
  void updateText(const UserInputState& state) {
    // Update the search text to reflect the new input
    _searchText.setString(state.getText());

    // Get the index of the glyph representing the character currently selected
    // by the cursor
    std::size_t glyphIndex = state.getCursorIndex();

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

    // Update the window
    update();
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
  sf::RenderWindow* _window;
  const sf::Font _font;
  sf::Text _searchText;
  sf::RectangleShape _searchCursor;
};
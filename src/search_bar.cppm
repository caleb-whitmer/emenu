module;
#include <SFML/Graphics.hpp>
export module SearchBar;
import std;
import UserInputState;

// #define FONT_SIZE 20
#define CURSOR_WIDTH 2
#define SEARCH_TEXT_POS {40, 40}
#define SEARCH_TEXT_COLOR sf::Color::Black

// Internal class to manage a search result type
class __SearchResult final : public sf::Drawable, public sf::Transformable {
 public:
  __SearchResult( const sf::Font& font,
                  unsigned fontSize, 
                  unsigned width,
                  sf::Vector2f padding,
                  sf::Color fgColor,
                  sf::Color bgColor,
                  sf::String text )
  : _foreground{font, text, fontSize},
    _padding{padding} {

    // Calculate the offset to correct the positioning of the foreground
    _textCorrectionOffset = _foreground.getPosition() - 
                            _foreground.getGlobalBounds().position;

    // Calculate the size of the background based on the height of the text and
    // desired with as well as desired padding
    _background.setSize(sf::Vector2f{
      static_cast<float>(width), 
      (2.0f * padding.y) + _foreground.getGlobalBounds().size.y});

    // Set colors
    _foreground.setFillColor(fgColor);
    _background.setFillColor(bgColor);
  }
 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    // Offset the state transform by the transform of the class before drawing
    // the background
    states.transform = this->getTransform() * states.transform;
    target.draw(_background, states);

    // Further offset the state transform to correct the positioning of the text
    // and offset it by the desired padding
    states.transform.translate(_textCorrectionOffset + _padding);
    target.draw(_foreground, states);
  }

  sf::RectangleShape _background;
  sf::Text _foreground;
  sf::Vector2f _padding;
  sf::Vector2f _textCorrectionOffset;
};

export class SearchBar final {
 public:
  SearchBar(  sf::RenderWindow& window, 
              const sf::Font& font,
              unsigned fontSize,
              const std::vector<sf::String>& options  )
  : _window{&window},
    _searchText{font, "", fontSize},
    _searchCursor{{CURSOR_WIDTH, static_cast<float>(fontSize)}},
    _test{font, fontSize, 100, {10, 10}, sf::Color::Black, sf::Color::Magenta, "|Test!"} {

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
    _updateSearch();
  }

  /**
   * @brief      Input a control key to the search bar (i.e. arrow keys)
   *
   * @param[in]  k     The key to be inputted
   */
  void control(sf::Keyboard::Key k) {
    if (_state.addControl(k))
      _updateSearch();
  }

  /**
   * @brief      Update the user interface
   */
  void update() {
    _window->clear(sf::Color::White);

    _test.setPosition({50, 250});
    _window->draw(_test);

    _window->draw(_searchText);
    _window->draw(_searchCursor);

    _window->display();
  }
 private:
  
  /**
   * @brief      Update the text stored in the search bar
   */
  void _updateSearch() {
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

    /**
     * Fuzzy search stuff here
     */
  }

  UserInputState _state;
  sf::RenderWindow* _window;
  sf::Text _searchText;
  sf::RectangleShape _searchCursor;

  __SearchResult _test;
};
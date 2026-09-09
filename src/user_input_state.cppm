module;
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/String.hpp>
export module UserInputState;
import std;

#define TEXT_MAX_LENGTH 128
#define CURSOR '|'

export class UserInputState final {
 public:
  
  const std::pair<sf::String, std::size_t> getText() const {
    return std::make_pair(sf::String(_text_state), _cursor - _text_state);
  }

  /**
   * @brief      Adds a character to the user input state
   *
   * @param[in]  c     A character encoded in UTF-32 Unicode
   */
  void addChar(char32_t c) {
    // If the character is graphical or is a space and there is room to insert
    // the new character
    if ((std::isgraph(c) or std::isspace(c)) and !_isTextFull()) {
      // Place the character at the cursor before incrementing
      _placeAtCursor(c);
      _incrementCursor();
      return;
    }

    switch (c) {
    case BACKSPACE:
      // Decrement the cursor and then remove the character it is selecting
      _decrementCursor();
      _deleteAfterCursor();
      break;
    case DELETE:
      // Remove the character after the cursor
      _deleteAfterCursor();
      break;
    }
  }

  /**
   * @brief      Updates the user input state based on a control character
   *
   * @param[in]  c     A SFML Keyboard key value
   *
   * @return     True if the control key is handled, false otherwise
   */
  bool addControl(sf::Keyboard::Key c) {
    switch(c) {
    case sf::Keyboard::Key::Left:
      _decrementCursor();
      return true;
    case sf::Keyboard::Key::Right:
      _incrementCursor();
      return true;
    }

    // Return false if the control key is unhandled
    return false;
  }

  void print() {
    // Simulate typing in stdout
    char32_t* p = _text_state;
    while(1) {
      if (p == _cursor)
        std::printf("%c", CURSOR);
      if (*p)
        std::printf("%c", (char)*p);
      else break;
      if (*p) ++p;
    }
    std::printf("\n");
    std::flush(std::cout);
  }
 private:
  /**
   * @brief      Place a character at the cursor
   *
   * @param[in]  c     The character encoded in UTF-32 
   */
  void _placeAtCursor(char32_t c) {
    if (_cursor <= _text_state_end) {
      // Move the state from the cursor to the end up by one character
      std::memmove(&_cursor[1], &_cursor[0], (_text_state_end-_cursor)*sizeof(char32_t));
      // Insert the new character
      _cursor[0] = c;
    }
  }

  /**
   * @brief      Delete the character after the cursor 
   */
  void _deleteAfterCursor() {
    // Move the state from the cursor to the end down by one character
    std::memmove(&_cursor[0], &_cursor[1], (_text_state_end-_cursor+1)*sizeof(char32_t));
    // Nullify the last character
    _text_state_end[0] = 0;
  }

  /**
   * @brief      Move the cursor to the right by one character
   */
  void _incrementCursor() {
    // Prevent cursor from selecting a nullified character
    if (_cursor[0])
      ++_cursor;
  }

  /**
   * @brief      Move the cursor to the left by one character
   */
  void _decrementCursor() {
    // Prevent cursor from leaving array of characters
    if (_cursor > _text_state)
      --_cursor;
  }

  /**
   * @brief      Determines if the character array is full by checking if the
   *             very last character slot is occupied
   *
   * @return     True if full, False otherwise
   */
  bool _isTextFull() {
    return _text_state_end[0];
  }

  enum Key { BACKSPACE=0x08, DELETE=0x7F };
  char32_t _text_state[TEXT_MAX_LENGTH] = { 0 };
  char32_t* _text_state_end = &_text_state[TEXT_MAX_LENGTH-1];
  char32_t* _cursor = _text_state;
};

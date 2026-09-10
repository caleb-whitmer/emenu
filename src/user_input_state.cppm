module;
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/String.hpp>
export module UserInputState;
import std;

#define TEXT_MAX_LENGTH 64
#define CURSOR '|'

#define CONTROL_PRESSED sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)

export class UserInputState final {
 public:
  /**
   * @brief      Gets the length of the text entered by the user
   *
   * @return     The length of the input state
   */
  inline std::size_t getInputLength() const {
    // Copy the text state pointer
    const char32_t* tc = _text_state;
    // Increment until the null-terminator
    while(tc[0]) ++tc;
    // Return the index of the null-terminator
    return tc - _text_state;
  }

  /**
   * @brief      Get the index of the cursor
   *
   * @return     The index of the cursor
   */
  inline std::size_t getCursorIndex() const {
    return _cursor - _text_state;
  }

  /**
   * @brief      Sets the index of the cursor
   *
   * @param[in]  i     The desired index
   */
  void setCursorIndex(std::size_t i) {
    // If out of bounds of the array then place at the end of the array
    _cursor = (&_text_state[i] > _text_state_end + 1)
              ? _text_state_end + 1
              : &_text_state[i];
    // Back up to the final non-null character
    while (_cursor > _text_state and !_cursor[-1]) --_cursor;
  }

  /**
   * @brief      Return the character that the cursor is currently selecting
   *
   * @return     A constant reference to the character being selected by the
   *             cursor
   */
  inline const char32_t& getCharAtCursor() const {
    return _cursor[0];
  }

  /**
   * @brief      Gets the text stored in the input state
   *
   * @return     The text as an sf::String
   */
  inline const sf::String getText() const {
    return sf::String(_text_state);
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
      if (CONTROL_PRESSED) {
        std::size_t s = _decrementCursorWord();
        _deleteAfterCursor(s);
        break;
      }
      // Decrement the cursor and then remove the character it is selecting
      _decrementCursor();
      _deleteAfterCursor();
      break;
    case DELETE:
      if (CONTROL_PRESSED) {
        std::size_t i = getCursorIndex();
        std::size_t s = _incrementCursorWord();
        setCursorIndex(i);
        _deleteAfterCursor(s);


        break;
      }
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
    // Todo: Up / Down, Page Up / Down
    switch(c) {
    case sf::Keyboard::Key::Left:
      if (CONTROL_PRESSED)
        _decrementCursorWord();
      else
        _decrementCursor();
      return true;
    case sf::Keyboard::Key::Right:
      if (CONTROL_PRESSED)
        _incrementCursorWord();
      else
        _incrementCursor();
      return true;
    case sf::Keyboard::Key::Home:
      setCursorIndex(0);
      return true;
    case sf::Keyboard::Key::End:
      setCursorIndex(TEXT_MAX_LENGTH-1);
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
   * @brief      Delete an amount of characters after the cursor
   *
   * @param[in]  amount  The amount of characters to delete
   */
  void _deleteAfterCursor(std::size_t amount = 1) {
    // If the amount exceeds the limits of the array then bound it to the length
    // of the array
    char32_t* toptr = (&_cursor[amount] > _text_state_end)
                      ? _text_state_end + 1
                      : &_cursor[amount];

    // Move the state from the cursor to the end down by 'amount' characters
    // Delete:   from cursor  to cursor+x
    std::memmove(&_cursor[0], toptr, (_text_state_end-_cursor+1)*sizeof(char32_t));
    // Nullify the slots which the characters previously occupied
    std::memset((char32_t*)(_text_state_end-amount+1), 0, amount*sizeof(char32_t));
  }

  /**
   * @brief      Move the cursor to the right by one character
   */
  inline void _incrementCursor() {
    // Prevent cursor from selecting a nullified character
    if (_cursor[0])
      ++_cursor;
  }

  /**
   * @brief      Move the cursor to the left by one character
   */
  inline void _decrementCursor() {
    // Prevent cursor from leaving array of characters
    if (_cursor > _text_state)
      --_cursor;
  }

  /**
   * @brief      Move the cursor to the right until the end of a word
   *
   * @return     The number of characters incremented
   */
  std::size_t _incrementCursorWord() {
    char32_t* csave = _cursor;

    do {
      // Check for null-terminator early
      if (!_cursor[0]) break;
      // Otherwise increment the cursor until a space is reached.
      ++_cursor;
    }  while(!std::isspace(_cursor[0]));

    // Return the number of characters incremented
    return _cursor - csave;
  }

  /**
   * @brief      Move the cursor to the left until the beginning of a word
   *
   * @return     The number of characters decremented
   */
  std::size_t _decrementCursorWord() {
    char32_t* csave = _cursor;
    do {
      // Check for string beginning early
      if (_cursor <= _text_state) break;
      // Otherwise decrement the cursor until a space will be reached.
      --_cursor;
    }  while(!std::isspace(_cursor[-1]));
    // We can check the previous character here because the early check will
    // prevent us from checking outside the bounds of the string

    // Return the number of characters decremented
    return csave - _cursor;
  }

  /**
   * @brief      Determines if the character array is full by checking if the
   *             very last character slot is occupied
   *
   * @return     True if full, False otherwise
   */
  inline bool _isTextFull() {
    return _text_state_end[0];
  }

  enum Key { BACKSPACE=0x08, DELETE=0x7F };
  char32_t _text_state[TEXT_MAX_LENGTH] = { 0 };
  char32_t* _text_state_end = &_text_state[TEXT_MAX_LENGTH-2];
  // Subtract two from the max length to leave room for null-terminator at end
  char32_t* _cursor = _text_state;
};

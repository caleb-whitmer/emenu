module;
#include <SFML/Window/Keyboard.hpp>
export module UserInputState;
import std;

#define TEXT_MAX_LENGTH 128
#define CURSOR '|'

export class UserInputState {
 public:
  void placeAtCursor(char32_t c) {
    if (cursor <= text_state_end) {
      // Move the state from the cursor to the end up by one character
      std::memmove(&cursor[1], &cursor[0], (text_state_end-cursor)*sizeof(char32_t));
      // Insert the new character
      cursor[0] = c;
    }
  }

  void deleteAtCursor() {
    // Move the state from the cursor to the end down by one character
    std::memmove(&cursor[0], &cursor[1], (text_state_end-cursor+1)*sizeof(char32_t));
    // Nullify the last character
    text_state_end[0] = 0;
  }

  void incrementCursor() {
    // Prevent cursor from selecting a nullified character
    if (cursor[0])
      ++cursor;
  }

  void decrementCursor() {
    // Prevent cursor from leaving array of characters
    if (cursor > text_state)
      --cursor;
  }

  bool textFull() {
    // Test if the text array is full by checking if the very last character is
    // null
    return text_state_end[0];
  }

  void addChar(char32_t c) {
    // If the character is graphical or is a space and there is room to insert
    // the new character
    if ((std::isgraph(c) or std::isspace(c)) and !textFull()) {
      // Place the character at the cursor before incrementing
      placeAtCursor(c);
      incrementCursor();
      return;
    }

    switch (c) {
    case BACKSPACE:
      // Decrement the cursor and then remove the character it is selecting
      decrementCursor();
      deleteAtCursor();
      break;
    case DELETE:
      // Remove the character after the cursor
      deleteAtCursor();
      break;
    }
  }

  bool addControl(sf::Keyboard::Key c) {
    switch(c) {
    case sf::Keyboard::Key::Left:
      decrementCursor();
      return true;
    case sf::Keyboard::Key::Right:
      incrementCursor();
      return true;
    }

    // Return false if the control key is unhandled
    return false;
  }

  void print() {
    // Simulate typing in stdout
    char32_t* p = text_state;
    while(1) {
      if (p == cursor)
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
  enum Key { BACKSPACE=0x08, DELETE=0x7F };
  char32_t text_state[TEXT_MAX_LENGTH] = { 0 };
  char32_t* text_state_end = &text_state[TEXT_MAX_LENGTH-1];
  char32_t* cursor = text_state;
};

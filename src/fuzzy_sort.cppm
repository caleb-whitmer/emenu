module;
#include <SFML/System/String.hpp>
export module FuzzySort;
import std;

/**
 * @brief      Compares to characters with a case sensitive equality check
 *
 * @param[in]  a     character
 * @param[in]  b     character
 *
 * @return     returns true if that characters match and false otherwise
 */
bool charCmpSensitive(char32_t a, char32_t b) {
  return a == b;
}

/**
 * @brief      Compares to characters with a case insensitive equality check
 *
 * @param[in]  a     character
 * @param[in]  b     character
 *
 * @return     returns true if the letters are the same or the characters match
 *             and false otherwise
 */
bool charCmpInsensitive(char32_t a, char32_t b) {
  return std::toupper(a) == std::toupper(b);
}

/**
 * @brief      Find the Levenshtein distance between two strings
 *
 * @param[in]  x          The string on the x-axis of the grid
 * @param[in]  y          The string on the y-axis of the grid
 * @param[in]  charMatch  The algorithm to determine if two given characters
 *                        match (defaults to the equality operator)
 *
 * @return     The Levenshtein distance based on the character matching
 *             algorithm
 */
unsigned __levDist( const sf::String& x, 
                    const sf::String& y, 
                    std::function<bool(char32_t, char32_t)> charMatch = 
                      charCmpSensitive ) {
  // Generate an array of the length of the string x +1 for the empty sub-string
  std::vector<unsigned> row(x.getSize() + 1, {});

  // Fill the row with incremented numbers
  unsigned i = 0;
  std::generate(row.begin(), row.end(), [&](){
    return i++;
  });

  unsigned nw, n;
  auto it = row.begin();

  // Loop through each row
  for (const auto& yc : y) {
    // Get an iterator for the row 
    it = row.begin();

    // Save the first column then increment to the next
    nw = (*it++)++;

    // Loop through each column
    for (const auto& xc : x) {
      // Save the current column
      n = *it;

      // Increment to the next column only after setting the current one based 
      // on the following rules:  1.  If the current cell matches the two 
      //                              characters, then set its value to the 
      //                              previous column of the previous row.
      //                          2.  Otherwise, set the value of the current 
      //                              cell to the one plus the minimum of the 
      //                              cells directly adjacent to the west, 
      //                              north, and northwest.
      (*it++) = charMatch(xc, yc) 
                ? nw 
                : 1 + std::min({nw, *it, *std::prev(it)});

      // Save the current column as the previous
      nw = n;
    }
  }

  // Return the final column of the final row as that is the levenshtein
  // distance
  return *(--row.end());
}

module;
#include <SFML/System/String.hpp>
export module FuzzySort;
import Settings;
import std;

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
                    Case charMatch = Case::Sensitive ) {
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

/**
 * @brief      Find the local Levenshtein distance between two strings. That is,
 *             find the minimum Levenshtein distance of the y-axis with every
 *             substring of the x-axis ending with the last character of x.
 *
 * @param[in]  x          The string on the x-axis of the grid
 * @param[in]  y          The string on the y-axis of the grid
 * @param[in]  charMatch  The algorithm to determine if two given characters
 *                        match (defaults to the equality operator)
 *
 * @return     The minimum distance between the y-string and all the ending
 *             sub-strings of the x-string
 */
unsigned __localLevDist(  const sf::String& x, 
                          const sf::String& y, 
                          Case charMatch = Case::Sensitive ) {
  // Generate an array of the length of the string x +1 for the empty sub-string
  std::vector<unsigned> row(x.getSize() + 1, {});

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

  // Return the minimum element of the final row of the local Levenshtein
  // distance grid
  return *std::min_element(row.begin(), row.end());
}

/**
 * @brief      Build a string comparison algorithm for fuzzy searching
 *
 * @param[in]  pattern  The pattern to search for
 * @param[in]  charCmp  The character comparison algorithm
 *
 * @return     An algorithm for sorting strings based on a fuzzy comparison to a
 *             given pattern
 */
export std::function<bool(const sf::String&, const sf::String&)> 
  fuzzyCmp( const sf::String& pattern, 
            Case charCmp = Case::Sensitive) {

  return [&](const sf::String& a, const sf::String& b){
    // Get the local Levenshtein distances of the two given strings
    // against the pattern
    unsigned llda = __localLevDist(a, pattern, charCmp);
    unsigned lldb = __localLevDist(b, pattern, charCmp);
    // If the local Levenstein distances are equal then default to the
    // proper Levenshtein distance
    if (llda == lldb)
      return __levDist(a, pattern, charCmp) < __levDist(b, pattern, charCmp);
    // Otherwise return the comparison of the locals
    return llda < lldb;
  };
}

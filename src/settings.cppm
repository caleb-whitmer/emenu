module;
#include <SFML/Graphics.hpp>
#include "ProggyClean.h"
export module Settings;
import std;

/**
 * @brief      Origin setting for positioning objects
 */
export enum class Origin {
  NorthWest,
  NorthEast,
  SouthEast,
  SouthWest,
  North,
  East,
  South,
  West,
  Center
};

/**
 * @brief      Almost acts like an enum class where a prefered comparison
 *             function can be chose with the syntax:
 *                    `auto c = Case::Sensitive;`
 *             However, the actual value of the alias is a function which 
 *             compares two chars.
 */
export struct Case final : public std::function<bool(char32_t, char32_t)> {
  /*
   * @brief      Convert a function reference to this type
   *
   * @param[in]  in    The function reference to be converted
   */
  Case(const auto& in) : std::function<bool(char32_t, char32_t)>{in} {}

  /**
   * @brief      Compares two characters with a case sensitive equality check
   *
   * @param[in]  a     character
   * @param[in]  b     character 
   *
   * @return     returns true if that characters match and false otherwise
   */
  static bool Sensitive(char32_t a, char32_t b) {
    return a == b;
  }

  /**
   * @brief      Compares two characters with a case insensitive equality check
   *
   * @param[in]  a     character
   * @param[in]  b     character
   *
   * @return     returns true if the letters are the same or the characters match
   *             and false otherwise
   */
  static bool Insensitive(char32_t a, char32_t b) {
    return std::toupper(a) == std::toupper(b);
  }
};

export struct Settings {
  Settings()
  : font{ProggyClean_ttf_raw, ProggyClean_ttf_raw_len} {}

  Settings(const std::filesystem::path& fontPath) 
  : font{fontPath} {}

  const sf::Font font           ;
  unsigned fontSize             {14};
  unsigned width                {600};
  sf::Vector2f padding          {16, 6};
  sf::Vector2i position         {0, 0};
  Origin origin                 {Origin::NorthWest};
  sf::Color foreground          {sf::Color::Black};
  sf::Color background          {sf::Color::White};
  sf::Color foregroundSelected  {sf::Color::White};
  sf::Color backgroundSelected  {sf::Color::Black};
  sf::Color promptColor         {sf::Color::Red};
  sf::String prompt             {""};
  unsigned lineCount            {10};
  Case case_                    {Case::Sensitive};
};

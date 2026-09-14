module;
#include <SFML/Graphics.hpp>
export module TextBox;

export class TextBox : public sf::Drawable, public sf::Transformable {
 public:
  TextBox(  const sf::Font& font,
            unsigned fontSize, 
            unsigned width,
            sf::Vector2f padding,
            sf::Color fgColor,
            sf::Color bgColor,
            sf::String text  )
  : _foreground{font, "|", fontSize}, // Kinda screwy. I need to initiate it 
                                      // with a full height character to 
                                      // calculate the correction offset
    _padding{padding} {


    // Calculate the offset to correct the positioning of the foreground
    _textCorrectionOffset = _foreground.getPosition() - 
                            _foreground.getGlobalBounds().position;

    // Set the actual text of the textbox
    _foreground.setString(text);

    // Calculate the size of the background based on the height of the text and
    // desired with as well as desired padding
    _background.setSize(sf::Vector2f{
      static_cast<float>(width), 
      (2.0f * padding.y) + fontSize});

    // Set colors
    _foreground.setFillColor(fgColor);
    _background.setFillColor(bgColor);
  }

  /**
   * @brief      Gets the size of the text box
   *
   * @return     The size of the text box
   */
  inline sf::Vector2f getSize() const {
    return _background.getSize();
  }

  /**
   * @brief      Gets the string currently stored in the text box
   *
   * @return     The string in the text box
   */
  inline const sf::String& getString() const {
    return _foreground.getString();
  } 

  /**
   * @brief      Sets the string in the text box
   *
   * @param[in]  str   The new string to insert
   */
  inline void setString(const sf::String& str) {
    _foreground.setString(str);
  }

 protected:
  // Constant references to allow get access but not set access
  const sf::Text& foreground                = _foreground;
  const sf::RectangleShape& background      = _background;
  const sf::Vector2f& textCorrectionOffset  = _textCorrectionOffset;
  const sf::Vector2f& padding               = _padding;

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


 private:
  sf::Text _foreground;
  sf::RectangleShape _background;
  sf::Vector2f _textCorrectionOffset;
  sf::Vector2f _padding;
};

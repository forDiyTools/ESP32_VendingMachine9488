#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

class Button {
  private:
    uint16_t posX, posY, width, height;
    uint8_t value;
  public:
    Button() {      
    }
    Button(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height, uint8_t _value) {
      posX = _x;
      posY = _y;
      width = _width;
      height = _height;
      value = _value;
    }
    
    boolean isClicked(uint16_t x, uint16_t y) {
      return x >= posX && y >= posY && x <= (posX + width) && y <= (posY + height);
    }

    uint8_t getValue() {
      return value;
    }
    uint16_t getX() {
      return posX;
    }
    uint16_t getY() {
      return posY;
    }
    uint16_t getWidth() {
      return width;
    }
    uint16_t getHeight() {
      return height;
    }
};

#endif /* INC_BUTTON_H_ */

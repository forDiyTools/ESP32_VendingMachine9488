#ifndef INC_FONTS_H_
#define INC_FONTS_H_

typedef struct {
  const uint8_t *data;
  uint16_t width;
  uint16_t height;
  uint8_t dataSize;
} tImage;

typedef struct {
  long int code;
  const tImage *image;
} tChar;

typedef struct {
  int length;
  const tChar *chars;
} tFont;

typedef struct {
  const uint8_t length;
  const uint8_t *chars;
} tText;

#endif /* INC_FONTS_H_ */

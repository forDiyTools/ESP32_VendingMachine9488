// pins below are defined on board LilyGo LilyPi, you can redefine it to the proper numeric values
#define INT_PIN 25
#define RST_PIN -1  // if absent
#define LILYGO_LILYPI false  //true only if config data is available

void handleTouch(int8_t contacts, GTPoint* points) {
  Serial.printf("Contacts: %d\n", contacts);
  touchState.isDown = true;
  // horizontal
  //touchState.posX = LCD_WIDTH - points[0].y;
  //touchState.posY = points[0].x;
  // vertical
  touchState.posX = LCD_WIDTH - points[0].x;
  touchState.posY = LCD_HEIGHT - points[0].y;
  //LCD_Point(touchState.posX, touchState.posY);
}

void touchStart() {
  unsigned short configInfo;
  
  touch.begin(INT_PIN, RST_PIN, GOODIX_I2C_ADDR_BA); // GOODIX_I2C_ADDR_28 GOODIX_I2C_ADDR_BA);
  Serial.print("Check ACK on addr request on 0x");
  Serial.print(touch.i2cAddr, HEX);
  Wire.beginTransmission(touch.i2cAddr);
  if (!Wire.endTransmission()) {
    Serial.println(": SUCCESS");
  } else {
    Serial.print(": ERROR!");
      touch.begin(INT_PIN, RST_PIN, GOODIX_I2C_ADDR_28); // GOODIX_I2C_ADDR_28 GOODIX_I2C_ADDR_BA);
      Serial.print("Check ACK on addr request on 0x");
      Serial.print(touch.i2cAddr, HEX);
      Wire.beginTransmission(touch.i2cAddr);
      if (!Wire.endTransmission()) {
        Serial.println(": SUCCESS");
      } else {
        Serial.print(": ERROR!");
      }
  }
  configInfo = touch.configCheck(LILYGO_LILYPI);
  if (!configInfo)
  {
    Serial.println("Config is OK");
  }
  else
  {
    Serial.print("Config ERROR: ");
    Serial.println(configInfo);
  }
}

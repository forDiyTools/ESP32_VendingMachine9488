#include <SPI.h>
#include <Wire.h>
#include "Goodix.h"
#include "fonts.h"
#include "button.h"

/************************************************************************/

// pin for coins
#define COIN_GPIO             35

// products numbers
#define PRODUCT_NUM_1         26
#define PRODUCT_NUM_2         41
#define PRODUCT_NUM_3         55
#define PRODUCT_NUM_4         62

#define MOTOR_PIN_1           2
#define MOTOR_PIN_2           4
#define MOTOR_PIN_3           18
#define MOTOR_PIN_4           23

/***********************************************************************/


#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SCLK   14
#define HSPI_CS     15

#define HSPI_DC     33
#define HSPI_RES    32

#define HSPI_LED    19

#define LCD_WIDTH   320
#define LCD_HEIGHT  480

#define COLOR_BLACK        0x000000
#define COLOR_WHITE        0xffffff
#define COLOR_RED          0xff0000
#define COLOR_GREEN        0x00ff00
#define COLOR_YELLOW       0xFFC300

#define COLOR_BUTT_BG      COLOR_YELLOW // 0xF1C40F
#define COLOR_BUTT_DOWN    COLOR_GREEN

typedef struct {
  bool isDown;
  bool isTriggered;
  bool isClicked;
  uint16_t posX;
  uint16_t posY;
} TouchStateStruct;

TouchStateStruct touchState;

typedef struct {
  bool isTriggered;
  uint8_t delay;
  uint8_t count;
  bool needRepaint;  
} CoinStateStruct;

CoinStateStruct coinState;

typedef enum {
 SCREEN_VERTICAL_1,
 SCREEN_HORIZONTAL_1,
 SCREEN_VERTICAL_2,
 SCREEN_HORIZONTAL_2
} LCDScreenRotation;

static const uint32_t spiClk = 40000000; // 24 MHz

extern const tFont fontVerdanaBold22;
extern const tFont fontMontserratBold26;
extern const tImage imageCandy1;
extern const tImage imageCoin;
extern const tImage imageSmile;
extern const tImage imageWarning;

SPIClass * hspi = NULL;

Goodix touch = Goodix();

Button button[12];

uint8_t productNum = 0;

void handleTouch(int8_t contacts, GTPoint* points);
void IRAM_ATTR handleCoin();


void setup() {
  // prepare coin pin
  pinMode(COIN_GPIO, INPUT_PULLUP);
  
  // prepare motors
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(MOTOR_PIN_3, OUTPUT);
  pinMode(MOTOR_PIN_4, OUTPUT);
  digitalWrite(MOTOR_PIN_1, LOW);
  digitalWrite(MOTOR_PIN_2, LOW);
  digitalWrite(MOTOR_PIN_3, LOW);
  digitalWrite(MOTOR_PIN_4, LOW);
  
  Serial.begin(115200);
  Serial.println("\nStarting...");

  pinMode(HSPI_DC, OUTPUT);
  pinMode(HSPI_RES, OUTPUT);

  digitalWrite(HSPI_RES, LOW);

  hspi = new SPIClass(HSPI);
  hspi->begin();
  pinMode(HSPI_CS, OUTPUT);

  LCD_Init();

  LCD_Rotation(SCREEN_VERTICAL_2);

  Wire.setClock(400000);
  Wire.begin();
  delay(300);
  touch.setHandler(handleTouch);
  touchStart();

  attachInterrupt(COIN_GPIO, handleCoin, FALLING);

  initButtons();
  prepareMainScreen();
  
  // turn on display light
  pinMode(HSPI_LED, OUTPUT);
  digitalWrite(HSPI_LED, HIGH);

  Serial.println("Service started");
}

void loop() {
  delay(10);
  touch.loop();
  if (touchState.isDown) {
    if (!touchState.isTriggered) {
      drawButtons(checkButtons());
    }
    touchState.isTriggered = true;
  } else {
    if (touchState.isTriggered) {
      touchState.isTriggered = false;
      touchState.isClicked = true;
      drawButtons(-1);
    }
  }
  touchState.isDown = false;
  
  if (touchState.isClicked) {
    touchState.isClicked = false;
    int8_t buttValue = checkButtons();
    if (buttValue >= 0) {
      if (buttValue == 32 && productNum > 0) {
        // enter
        prepareProduct();
      } else if (buttValue == 33) {
        // cancel
        productNum = 0;                     
        showNumber();
      } else if (productNum < 10 && buttValue < 10) {
        productNum = productNum * 10 + buttValue;
        showNumber();
      }
    }    
  }

  if (coinState.isTriggered) {
    if (coinState.delay > 0) {
      coinState.delay--;
    } else {
      if (coinState.count < 99) {
        coinState.count++;
        coinState.needRepaint = true;
      }
      coinState.isTriggered = false;     
    }
  }

  if (coinState.needRepaint) {
    coinState.needRepaint = false;        
    drawCoinCount();
  }
}

void IRAM_ATTR handleCoin() {
  coinState.isTriggered = true;
  coinState.delay = 80;  
}

void showNumber() {
  uint16_t x = 0;
  x = LCD_DrawText(fontVerdanaBold22, "Product: ", x, 65, COLOR_WHITE, COLOR_BLACK);
  x += LCD_DrawChar(fontMontserratBold26, productNum / 10 + 48, x, 65, COLOR_WHITE, COLOR_BLACK);
  x += LCD_DrawChar(fontMontserratBold26, (productNum % 10) + 48, x, 65, COLOR_WHITE, COLOR_BLACK);
  LCD_DrawChar(fontMontserratBold26, 32, x, 65, COLOR_WHITE, COLOR_BLACK);
}

void drawCoinCount() {
  uint16_t x = 180;
  x = LCD_DrawText(fontVerdanaBold22, "Coins: ", x, 65, COLOR_WHITE, COLOR_BLACK);
  x += LCD_DrawChar(fontMontserratBold26, coinState.count / 10 + 48, x, 65, COLOR_WHITE, COLOR_BLACK);
  x += LCD_DrawChar(fontMontserratBold26, (coinState.count % 10) + 48, x, 65, COLOR_WHITE, COLOR_BLACK);
  LCD_DrawChar(fontMontserratBold26, 32, x, 65, COLOR_WHITE, COLOR_BLACK);
}

void prepareMainScreen() {
  LCD_FillScreen(COLOR_BLACK);
  LCD_DrawText(fontVerdanaBold22, "Insert coin", 0, 0, COLOR_WHITE, COLOR_BLACK);
  LCD_DrawText(fontVerdanaBold22, "and select product", 80, 22, COLOR_WHITE, COLOR_BLACK);

  productNum = 0;
  drawButtons(-1);
  showNumber();
  drawCoinCount();
}

void initButtons() {
  uint8_t width = 90;
  uint8_t height = 60;  
  button[0] = Button(10, 130, width, height, 1);
  button[1] = Button(110, 130, width, height, 2);
  button[2] = Button(210, 130, width, height, 3);
  
  button[3] = Button(10, 220, width, height, 4);
  button[4] = Button(110, 220, width, height, 5);
  button[5] = Button(210, 220, width, height, 6);
  
  button[6] = Button(10, 310, width, height, 7);
  button[7] = Button(110, 310, width, height, 8);
  button[8] = Button(210, 310, width, height, 9);

  button[9] = Button(110, 400, width, height, 0);
  // enter
  button[10] = Button(10, 400, width, height, 33);
  // cancel
  button[11] = Button(210, 400, width, height, 32);
}

int8_t checkButtons() {
  for (int i = 0; i < 12; i++) {
    if (button[i].isClicked(touchState.posX, touchState.posY)) {
      return button[i].getValue();
    }
  }
  return -1;
}

void drawButtons(int8_t active) {
  uint32_t bgColor;
  for (int i = 0; i < 12; i++) {
    bgColor = button[i].getValue() == active ? COLOR_BUTT_DOWN : COLOR_BUTT_BG;

    LCD_Rectangle(button[i].getX(), button[i].getY(), button[i].getWidth(), button[i].getHeight(), bgColor);
    if (button[i].getValue() < 10) {
      LCD_DrawChar(fontVerdanaBold22, 48 + button[i].getValue(), button[i].getX() + 15, button[i].getY() + 5, COLOR_BLACK, bgColor);
    }
    if (button[i].getValue() == 32) {
      LCD_DrawText(fontVerdanaBold22, "Enter", button[i].getX() + 3, button[i].getY() + 5, COLOR_BLACK, bgColor);
    }
    if (button[i].getValue() == 33) {
      LCD_DrawText(fontVerdanaBold22, "Cancel", button[i].getX() + 3, button[i].getY() + 5, COLOR_BLACK, bgColor);
    }
  }
}

void prepareProduct() {
  int8_t num = -1;
  switch (productNum) {
    case PRODUCT_NUM_1:
      num = 0;
      break;
    case PRODUCT_NUM_2:
      num = 1;
      break;
    case PRODUCT_NUM_3:
      num = 2;
      break;
    case PRODUCT_NUM_4:
      num = 3;
      break;
    default:
      LCD_Rectangle(0, 0, 320, 480, COLOR_BLACK);
      LCD_DrawText(fontVerdanaBold22, "Wrong Product Number!", 0, 110, COLOR_WHITE, COLOR_BLACK);
      LCD_DrawImage(96, 160, imageWarning);
      delay(3000);
      prepareMainScreen();
      return;
      break;
  }

  if (coinState.count == 0) {
      LCD_Rectangle(0, 0, 320, 480, COLOR_BLACK);
      LCD_DrawText(fontVerdanaBold22, "Not enough coins!", 50, 110, COLOR_RED, COLOR_BLACK);
      LCD_DrawImage(96, 160, imageCoin);
      delay(3000);
      prepareMainScreen();
      return;
  }
  coinState.count--;
  
  LCD_Rectangle(0, 0, 320, 480, COLOR_BLACK);
  LCD_DrawText(fontVerdanaBold22, "Product", 30, 50, COLOR_WHITE, COLOR_BLACK);
  uint16_t x = 60;
  x += LCD_DrawChar(fontMontserratBold26, productNum / 10 + 48, x, 90, COLOR_WHITE, COLOR_BLACK);
  x += LCD_DrawChar(fontMontserratBold26, (productNum % 10) + 48, x, 90, COLOR_WHITE, COLOR_BLACK);
  LCD_DrawText(fontVerdanaBold22, "is preparing", 90, 130, COLOR_WHITE, COLOR_BLACK);
  LCD_DrawImage(96, 180, imageCandy1);
  delay(1000);
  
  activateMotor(num);
  
  delay(500);

  LCD_Rectangle(0, 0, 320, 480, COLOR_BLACK);
  LCD_DrawText(fontVerdanaBold22, "Thank you!", 95, 100, COLOR_WHITE, COLOR_BLACK);
  LCD_DrawImage(96, 180, imageSmile);
  delay(3000);
  prepareMainScreen();
}

void activateMotor(uint8_t num) {
  switch (num) {
    case 0:
      digitalWrite(MOTOR_PIN_1, HIGH);
      break;
    case 1:
      digitalWrite(MOTOR_PIN_2, HIGH);
      break;
    case 2:
      digitalWrite(MOTOR_PIN_3, HIGH);
      break;
    case 3:
      digitalWrite(MOTOR_PIN_4, HIGH);
      break;
  }
  delay(400);
  digitalWrite(MOTOR_PIN_1, LOW);
  digitalWrite(MOTOR_PIN_2, LOW);
  digitalWrite(MOTOR_PIN_3, LOW);
  digitalWrite(MOTOR_PIN_4, LOW);
  delay(1000);
}

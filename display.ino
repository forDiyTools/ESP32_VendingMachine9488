#define BURST_MAX_SIZE    900

uint8_t burst_buffer[BURST_MAX_SIZE + 3];

void LCD_Init() {
  digitalWrite(HSPI_CS, HIGH);
  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));

  digitalWrite(HSPI_CS, LOW);
  // LCD_PrepareDisplay();
  LCD_PrepareDisplayAlt();
  
  digitalWrite(HSPI_CS, HIGH); 
}

void LCD_Deinit() {
  hspi->endTransaction();
  digitalWrite(HSPI_CS, HIGH);
}

void LCD_WR_CMD(int data) {
  digitalWrite(HSPI_DC, LOW);

  digitalWrite(HSPI_CS, LOW);
  hspi->transfer(data);
  digitalWrite(HSPI_CS, HIGH);
}

void LCD_WR_DATA(int data) {
  digitalWrite(HSPI_DC, HIGH);
  
  digitalWrite(HSPI_CS, LOW);
  hspi->transfer(data);
  digitalWrite(HSPI_CS, HIGH);
}

void LCD_PrepareDisplayAlt() {
  delay(100);
  digitalWrite(HSPI_RES, HIGH);
  delay(200);

//************* ILI9488іхКј»Ї**********//	
	LCD_WR_CMD(0XF7);
	LCD_WR_DATA(0xA9);
	LCD_WR_DATA(0x51);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x82);
	LCD_WR_CMD(0xC0);
	LCD_WR_DATA(0x11);
	LCD_WR_DATA(0x09);
	LCD_WR_CMD(0xC1);
	LCD_WR_DATA(0x41);
	LCD_WR_CMD(0XC5);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x80);
	LCD_WR_CMD(0xB1);
	LCD_WR_DATA(0xB0);
	LCD_WR_DATA(0x11);
	LCD_WR_CMD(0xB4);
	LCD_WR_DATA(0x02);
	LCD_WR_CMD(0xB6);//їШЦЖ·ЅПт
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x22);//0X42ёДіЙ0X22
	LCD_WR_CMD(0xB7);
	LCD_WR_DATA(0xc6);
	LCD_WR_CMD(0xBE);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x04);
	LCD_WR_CMD(0xE9);
	LCD_WR_DATA(0x00);
	LCD_WR_CMD(0x36);
	LCD_WR_DATA((1<<3)|(0<<7)|(1<<6)|(1<<5));
	LCD_WR_CMD(0x3A);
	LCD_WR_DATA(0x66);
	LCD_WR_CMD(0xE0);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x41);
	LCD_WR_DATA(0x89);
	LCD_WR_DATA(0x4B);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x0C);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x18);
	LCD_WR_DATA(0x1B);
	LCD_WR_DATA(0x0F);
	LCD_WR_CMD(0XE1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x17);
	LCD_WR_DATA(0x1A);
	LCD_WR_DATA(0x04);
	LCD_WR_DATA(0x0E);
	LCD_WR_DATA(0x06);
	LCD_WR_DATA(0x2F);
	LCD_WR_DATA(0x45);
	LCD_WR_DATA(0x43);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x36);
	LCD_WR_DATA(0x0F);
	//LCD_WR_CMD(0x11);
	//delay_ms(120);
	//LCD_WR_CMD(0x29);

  LCD_WR_CMD(0x11); //Sleep out
  delay(120);
  LCD_WR_CMD(0x29); //Display on 
}

void LCD_PrepareDisplay(void) {
	//LCD_GPIO_Init();//Init the GPIOs for LCD
	//LCD_RESET();//You should ResetLCD before Init.
  delay(100);
  digitalWrite(HSPI_RES, HIGH);
  delay(200);

  //************* Start Initial Sequence **********//
  LCD_WR_CMD(0x3A); // Interface Pixel Format
  LCD_WR_DATA(0x66); //18bit
    
  LCD_WR_CMD(0xF7); // Adjust Control
  LCD_WR_DATA(0xA9);
  LCD_WR_DATA(0x51);
  LCD_WR_DATA(0x2C);
  LCD_WR_DATA(0x82); // D7 stream, loose

  LCD_WR_CMD(0XC0); //Power Control 1
  LCD_WR_DATA(0x11); //Vreg1out
  LCD_WR_DATA(0x09); //Verg2out

  LCD_WR_CMD(0xC1); //Power Control 2
  LCD_WR_DATA(0x41); //VGH,VGL
  
  LCD_WR_CMD(0xC5); //Power Control 3
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x0A); //Vcom
  LCD_WR_DATA(0x80);
  
  LCD_WR_CMD(0xB1); //Frame rate
  LCD_WR_DATA(0xA0); // 0xA0 - 60Hz
  LCD_WR_DATA(0x11); // 0x11 - 60Hz
  
  LCD_WR_CMD(0xB4); //Display Inversion Control
  LCD_WR_DATA(0x02); //2-dot
  
  LCD_WR_CMD(0XB6); //RGB/MCU Interface Control
  LCD_WR_DATA(0x02); //MCU
  LCD_WR_DATA(0x22); //Source,Gate scan dieection

  LCD_WR_CMD(0XB7); //Entry Mode Set
  LCD_WR_DATA(0xC6);
  
  LCD_WR_CMD(0xBE); // HS Lanes Contro
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x04);

  LCD_WR_CMD(0XE9); // Set Image Function
  LCD_WR_DATA(0x00); //disable 24 bit data input

  LCD_WR_CMD(0x36); //Memory Access
  LCD_WR_DATA(0xa8); // 0x28
  
  
  LCD_WR_CMD(0xE0); //P-Gamma
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x07);
  LCD_WR_DATA(0x10);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x41);
  LCD_WR_DATA(0x89);
  LCD_WR_DATA(0x4B);
  LCD_WR_DATA(0x0A);
  LCD_WR_DATA(0x0C);
  LCD_WR_DATA(0x0E);
  LCD_WR_DATA(0x18);
  LCD_WR_DATA(0x1B);
  LCD_WR_DATA(0x0F);
  
  LCD_WR_CMD(0XE1); //N-Gamma
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x1A);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x0E);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x2F);
  LCD_WR_DATA(0x45);
  LCD_WR_DATA(0x43);
  LCD_WR_DATA(0x02);
  LCD_WR_DATA(0x0A);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x32);
  LCD_WR_DATA(0x36);
  LCD_WR_DATA(0x0F);
  
  //LCD_WR_CMD(0XB0); // Interface Mode Control
  //LCD_WR_DATA(0x00);
  
  //LCD_WR_CMD(0x21); //Normal Black
  LCD_WR_CMD(0x11); //Sleep out
  delay(120);
  LCD_WR_CMD(0x29); //Display on 
}

// Set rotation of the screen - changes x0 and y0
void LCD_Rotation(LCDScreenRotation rotation) {
  LCD_WR_CMD(0x36);
  delay(1);

  switch(rotation) {
		case SCREEN_VERTICAL_1:
			LCD_WR_DATA(0x40|0x08);
			break;
		case SCREEN_HORIZONTAL_1:
			LCD_WR_DATA(0x20|0x08);
			break;
		case SCREEN_VERTICAL_2:
			LCD_WR_DATA(0x80|0x08|0x40); // |0x40 - direction
			break;
		case SCREEN_HORIZONTAL_2:
			LCD_WR_DATA(0x40|0x80|0x20|0x08);
			break;
		default:
			//EXIT IF SCREEN ROTATION NOT VALID!
			break;
  }
}

void LCD_WriteData(uint8_t* buff, uint32_t buff_size) {
  digitalWrite(HSPI_DC, HIGH); 
  digitalWrite(HSPI_CS, LOW);
  hspi->transfer(buff, buff_size);
  digitalWrite(HSPI_CS, HIGH);
}

void LCD_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {		
	LCD_WR_CMD(0x2A);	
	LCD_WR_DATA(x1>>8);
	LCD_WR_DATA(0x00FF&x1);		
	LCD_WR_DATA(x2>>8);
	LCD_WR_DATA(0x00FF&x2);

	LCD_WR_CMD(0x2B);	
	LCD_WR_DATA(y1>>8);
	LCD_WR_DATA(0x00FF&y1);		
	LCD_WR_DATA(y2>>8);
	LCD_WR_DATA(0x00FF&y2);
	LCD_WR_CMD(0x2C);	//Start writing to the GRAM			
}

void LCD_DrawColourBurst(uint32_t Color, uint32_t Size) {
  //Serial.println("DrawColourBurst 1");
  //SENDS COLOR
  uint32_t Buffer_Size = 0;
  uint32_t Sending_Size = Size * 3;
  if(Sending_Size < BURST_MAX_SIZE) {
    Buffer_Size = Sending_Size;
  } else {
    Buffer_Size = BURST_MAX_SIZE;
  }
  //Serial.println("DrawColourBurst 2");

  digitalWrite(HSPI_DC, HIGH);
  digitalWrite(HSPI_CS, LOW);

  uint8_t chifted0 = Color >> 16;
  uint8_t chifted = Color >> 8;
  uint8_t chifted1 = Color & 0xff;
  uint8_t burst_buffer[Buffer_Size+100];
  
  //Serial.println("DrawColourBurst 3");

  for (uint32_t j = 0; j < Buffer_Size; j+=3) {
    burst_buffer[j] = chifted0;
    burst_buffer[j + 1] = chifted;
    burst_buffer[j + 2] = chifted1;
  }

  //Serial.println("DrawColourBurst 4");

  uint32_t Sending_in_Block = Sending_Size / Buffer_Size;
  uint32_t Remainder_from_block = Sending_Size % Buffer_Size;

  //Serial.println("DrawColourBurst 5");
  
  if(Sending_in_Block != 0) {
    for(uint32_t j = 0; j < (Sending_in_Block); j++) {
      //LCD_WriteData(burst_buffer, Buffer_Size);
      hspi->writeBytes(burst_buffer, Buffer_Size);
    }
  }
  //Serial.println("DrawColourBurst 6");
  if (Remainder_from_block > 0) {
	  //REMAINDER!
	  //LCD_WriteData(burst_buffer, Remainder_from_block);
      hspi->writeBytes(burst_buffer, Remainder_from_block);
  }
  
  //Serial.println("DrawColourBurst 7");
  
  digitalWrite(HSPI_CS, HIGH);
}

void LCD_Point(int x, int y) {
  LCD_SetAddress(x, y, x + 1, y + 1);
  LCD_WR_DATA(255);
  LCD_WR_DATA(255);
  LCD_WR_DATA(255);    
}

void LCD_FillScreen(uint32_t Color) {
  digitalWrite(HSPI_CS, LOW);  
  LCD_SetAddress(0, 0, LCD_WIDTH, LCD_HEIGHT);
  LCD_DrawColourBurst(Color, LCD_WIDTH * LCD_HEIGHT);
  digitalWrite(HSPI_CS, HIGH);
}

uint8_t LCD_DrawChar(tFont font, long int character, uint16_t xx, uint16_t yy, uint32_t color, uint32_t bgColor) {
  uint8_t charNum = 0;
  for (uint8_t f = 0; f < font.length; f++) {
    if (font.chars[f].code == character) {
      charNum = f;
      break;
    }
  }
  return LCD_Draw2BitImg(font.chars[charNum].image, xx, yy, color, bgColor);
}

uint16_t LCD_DrawText(tFont font, String text, uint16_t xx, uint16_t yy, uint32_t color, uint32_t bgColor) {
  for (int f = 0; f < text.length(); f++) {
    xx = xx + LCD_DrawChar(font, text[f], xx, yy, color, bgColor);
  }
  return xx;
}

uint8_t LCD_Draw2BitImg(const tImage *image, uint16_t xx, uint16_t yy, uint32_t color, uint32_t backgroundColor) {
  uint16_t pos = 0;
  uint8_t bpos = 7;
  uint16_t bufPos = 0;
 
  uint16_t height = image->height;
  uint32_t size = height * image->width * 3;

  uint8_t c2 = color >> 16;
  uint8_t c1 = color >> 8;
  uint8_t c0 = color & 0xff;
  uint8_t b2 = backgroundColor >> 16;
  uint8_t b1 = backgroundColor >> 8;
  uint8_t b0 = backgroundColor & 0xff;

  LCD_SetAddress(xx, yy, xx + image->width - 1, yy + height - 1);
  digitalWrite(HSPI_CS, LOW);
  digitalWrite(HSPI_DC, HIGH);
  
  for (char y = 0; y < height; y++) {
      for (char x = 0; x < image->width; x++) {
        if (image->data[pos] & (1 << bpos)) {
          burst_buffer[bufPos++] = c2;
          burst_buffer[bufPos++] = c1;
          burst_buffer[bufPos++] = c0;
        } else {
          burst_buffer[bufPos++] = b2;
          burst_buffer[bufPos++] = b1;
          burst_buffer[bufPos++] = b0;
        }
        if (bufPos >= BURST_MAX_SIZE) {
          hspi->writeBytes(burst_buffer, BURST_MAX_SIZE);
          bufPos = bufPos - BURST_MAX_SIZE;
          size = size - BURST_MAX_SIZE;
        }
        if (bpos == 0) {
          pos++;
          bpos = 7;
        } else {
          bpos--;
        }
      }
  }
  
  if (size > 0) {
    hspi->writeBytes(burst_buffer, size); 
  }

  digitalWrite(HSPI_CS, HIGH);
  uint8_t w = image->width;
  return w;
}

void LCD_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) {
    return;
  }
  if((x + width - 1) >= LCD_WIDTH) {
    width = LCD_WIDTH - x;
  }
  if((y + height - 1) >= LCD_HEIGHT) {
    height = LCD_HEIGHT - y;
  }
  LCD_SetAddress(x, y, x + width - 1, y + height - 1);
  LCD_DrawColourBurst(color, height * width);
}

void LCD_DrawImage(uint16_t x, uint16_t y, tImage image) {
  digitalWrite(HSPI_CS, LOW);  
  LCD_SetAddress(x, y, x + image.width - 1, y + image.height - 1);
  uint32_t buffSize = image.width * image.height * 3;
  for (uint32_t i = 0; i < buffSize; i++) {
    LCD_WR_DATA(image.data[i]);
  }
  digitalWrite(HSPI_CS, HIGH);
}

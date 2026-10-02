#include <PopetLCD.h>

uint16_t Voltage = 0;

void setup() {
  LCD_init();
}

void loop() {
  int raw = analogRead(A0);
  uint16_t Voltage = (uint32_t)raw * 500 / 1023;
  LCD_printInt(Voltage);
  LCD_setCursor(5, 0);
  LCD_print("V");
  _delay_ms(500);
  LCD_clear();
}
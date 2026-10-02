#ifndef POPETLCD_H
#define POPETLCD_H

#include <Arduino.h>

#define LCD_RS PB0
#define LCD_E  PB1

void LCD_command(uint8_t command_data, uint8_t Is_RS);
void LCD_init();
void LCD_data(char c);
void LCD_setCursor(uint8_t col, uint8_t row);
void LCD_print(const char *str);
void LCD_clear();
void LCD_printNum(uint16_t Num);
void LCD_printInt(uint16_t IntValue);

#endif
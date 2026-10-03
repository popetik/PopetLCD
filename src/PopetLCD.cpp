#include "PopetLCD.h"
#include <util/delay.h>

void LCD_command(uint8_t command_data, uint8_t Is_RS) { // Функция отправки команд/данных
  if (Is_RS == 1) { 
    PORTB |= (1 << LCD_RS);
  }
  else {
    PORTB &= ~(1 << LCD_RS);
  }
  _delay_us(1); // Даем дисплею время чтоб увидел наш сигнал RS

  PORTD = PORTD & 0x0F; // Сбрасываем в ноль старший ниббл
  PORTD = PORTD | (command_data & 0xF0); // Выставляем Логическую эдиницу в старшие 4 бита портов PD4, PD5, PD6, PD7 и не трогаем младшие биты PD3, PD2, PD1, PD0
  PORTB |= (1 << LCD_E); // Дергаем пин Е
  _delay_us(1); // Даем время дисплею чтоб увидел сигнал E
  PORTB &= ~(1 << LCD_E); // Опускаем пин Е
  _delay_us(1);

  PORTD = PORTD & 0x0F; // Сбрасываем в ноль младший ниббл
  PORTD = PORTD | ((command_data << 4) & 0xF0); // Сдвигаем на 4 бита и выставляем в старшие 4 бита портов
  PORTB |= (1 << LCD_E); // Дергаем пин Е
  _delay_us(1); // Даем время дисплею чтоб увидел сигнал E
  PORTB &= ~(1 << LCD_E); // Опускаем пин Е
  _delay_us(1);

  if (Is_RS == 0 && command_data <= 0x03) { // задержка для долгих команд, например очисткав и возвращения курсора на начальные координаты
    _delay_ms(2); // 2 мс для долгих команд
  }
  else {
    _delay_us(50); // 50 мкс для простых команд
  }
}

void LCD_printNum(uint16_t Num) { // функция вывода чисел
  if (Num >= 10) { // если в числе больше 1 цифры, то..
    LCD_printNum(Num / 10); // выводим старшие цифры
  }
  LCD_data('0' + Num % 10); // выводим последнюю цифру
}

void LCD_printInt(uint16_t IntValue) { // функция вывода точки в ХХ.ХХ формат
  uint16_t whole = IntValue / 100; // первые 2 цифры
  uint16_t frac = IntValue % 100; // последние 2 цифры
  if (whole < 10) LCD_data(' '); 
  LCD_printNum(whole); // вывести целую часть
  LCD_data('.'); // ставим точку между цифрами
  if (frac < 10) LCD_data('0');
  LCD_printNum(frac); // вывести последние 2 цифры
}

void LCD_clear() { // Функция очистки и возвращения курсора в самое начало дисплея
  LCD_command(0x01, 0); // Очищаем и возвращаем курсор
}

void LCD_nibble(uint8_t nibble) { // отправка старшего полубайта
  PORTD = PORTD & 0x0F;
  PORTD = PORTD | (nibble & 0xF0); // Выставляем Логическую эдиницу в старшие 4 бита портов PD4, PD5, PD6, PD7 и не трогаем младшие биты PD3, PD2, PD1, PD0
  PORTB |= (1 << LCD_E); // Дергаем пин Е
  _delay_us(1); // Даем время дисплею чтоб увидел сигнал E
  PORTB &= ~(1 << LCD_E); // Опускаем пин Е
  _delay_us(1);
}

void LCD_init() { // Функция перевода дисплей в 4 бит режим и отключения мигающего курсора
  DDRD |= 0xF0;
  DDRB |= (1 << LCD_RS) | (1 << LCD_E);
  PORTB &= ~((1 << LCD_RS) | (1 << LCD_E));   // RS = 0, E = 0

  _delay_ms(50);
  LCD_nibble(0x30);
  _delay_ms(5);
  LCD_nibble(0x30);
  _delay_us(150);
  LCD_nibble(0x30);
  _delay_us(150);
  LCD_nibble(0x20);          // переход в 4-битный режим
  _delay_us(150);

  LCD_command(0x28, 0);      // 4 бита, 2 строки
  LCD_command(0x0C, 0);      // дисплей включён, курсор выключен
  LCD_command(0x01, 0);      // очистка
  LCD_command(0x06, 0);      // курсор вправо
}

void LCD_data(char c) { // Функция удобного вывода одной буквы
  LCD_command(c, 1);
}

void LCD_setCursor(uint8_t col, uint8_t row) { // Функция установки курсора на нужный столбец и строку
  uint8_t address;
  if (row == 1) {
    address = 0x40 + col; // Выводим на вторую строку
  }
  else if (row == 2) {
    address = 0x14 + col;
  }
  else if (row == 3) {
    address = 0x54 + col;
  }
  else {
    address = col; // Выводим на первую строку
  }
  LCD_command(0x80 + address, 0); // Выводим итоговый результат
}

void LCD_print(const char *str) { // Функция удобного вывода текста
  uint8_t i = 0;
  while (str[i] != 0) {
    LCD_data(str[i]);
    i++;
  }
}

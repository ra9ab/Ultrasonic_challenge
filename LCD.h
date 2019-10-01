
#ifndef LCD_H_
#define LCD_H_


#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#define DATA_PORT PORTA
#define CMD_PORT PORTB

#define DATA_DDR DDRA
#define CMD_DDR DDRB
#define RS 1
#define EN 3
#define RW 2

void Enable();
void CMD_WRT_8bit(uint8_t cmd);
void CMD_WRT_4bit(uint8_t cmd);
void DATA_WRT_8bit(uint8_t data);
void DATA_WRT_4bit(uint8_t data);
void LCD_INIT_4bit();
void LCD_CLR();
void LCD_Cursor_Home();
void LCD_CUSRSOR_POS(uint8_t row , uint8_t col);
void LCD_CHAR_DISP(uint8_t row , uint8_t col , uint8_t data);
void LCD_String_DISP(uint8_t row , uint8_t col ,char *s);
void LCD_NUM_DISP(uint8_t row , uint8_t col ,int num);


#endif
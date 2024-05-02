#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "mbed.h"
#include <string>

const int DISPLAY_ADDR = 0x3C << 1; // LCD address
#define LCD_ROWS     2  // number of rows
#define LCD_COLS     16 // number of characters i.e. columns

void writeLCD(string text[]);
void writeLine(char lineNum, string data);
void writeData (char data);
void writeInst (char inst);
void initDisplay (void);
void clearDisplay (void);
void setCursor (char row, char col);


#endif

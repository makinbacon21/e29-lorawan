#include "lcd_i2c.h"

I2C i2c(I2C_SDA, I2C_SCL);

void writeLCD(string text[]){
  for (char j = 0; j < LCD_ROWS; j++){
    writeLine(j,text[j]);
  }
}

void writeLine(char lineNum, string data) {

  setCursor(lineNum,0);

  int dataLen = data.length();

  if (dataLen <= LCD_COLS){
    for (int j = 0 ; j < dataLen ; j++) {
      writeData(data[j]);
    }

    // clear the rest of the LCD row
    for (int j = dataLen; j < LCD_COLS; j++){
      writeData(' ');
    }
  }

}

void writeData (char data) {
    char cmd[2] = {0x40,data};

    char error = i2c.write(DISPLAY_ADDR,cmd,2);
    if (error) printf("error for data: %c\n",data);

}

void writeInst (char inst) {

    char cmd[2] = {0x0,inst};

    char error = i2c.write(DISPLAY_ADDR,cmd,2);
    if (error) printf("error for instr %x\n",inst);
}

void initDisplay (void) {
  
  thread_sleep_for(40); // wait for more than 40 ms
  
  writeInst(0x38); // DL=1: 8 bits; N=1: 2 line; F=0: 5 x 8 dots
  wait_us(100); // wait at least 100 us

  writeInst(0x0C); // D=1, display on; C=B=0; cursor off; blinking off;
  //writeInst(0x0F); // FOR DEBUGGING: cursor on, blinking on
  wait_us(100); // wait at least 100 us

  writeInst(0x01); // clear display
  thread_sleep_for(10); // wait at least 10 ms

  writeInst(0x06); // I/D=1: Increment by 1; S=0: No shift
  wait_us(100); // wait at least 100 us

}

void clearDisplay (void) {
  
  writeInst(0x01); // clear display
  thread_sleep_for(10); // wait at least 10 ms

}

void setCursor (char row, char col) {
  
  char addr;

  // first row addr is 0x00 to 0x0F, second row is 0x40 to 0x4F
  // B7 indicates DDRAM addr set
  addr = 0x80 | (row * 0x40 + col); 

  writeInst(addr);

}

// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
}

void loop(){
  serial.print("Hello FPGA! ");
  ledout.write(0);
  delay_ms(500);
  ledout.write(1);
  delay_ms(500);
}

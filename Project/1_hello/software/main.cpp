// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
}

void loop(){
  delay_ms(1000);
  serial.print("Hello FPGA!\n");
}

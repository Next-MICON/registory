// main.cpp
#include "firmware.hpp"

void init(){
  // Code Here!
  serial0.baud(9600);
}

void loop(){
  // Code Here!
  serial0.print("Hello FPGA!\n");
  delay_ms(1000);
}

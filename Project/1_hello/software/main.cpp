// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
  serial.print("Hello FPGA!\n");
}

void loop(){
  // Code Here!
}

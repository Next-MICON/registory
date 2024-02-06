// main.cpp
#include "firmware.hpp"

void init(){
  serial0.baud(9600);
  serial0.print("Hello FPGA!\n");
}

void loop(){
    // Code Here!
}

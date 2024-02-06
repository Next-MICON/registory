// main.cpp
#include "firmware.hpp"

void init(){
  serial0.baud(9600);
  serial0.print("Hello FPGA!\n");
  set_irq_mask(0);
  counter0.set(0x00000000);
}

void loop(){
  serial0.print("#");
}

void irq3(){
  serial0.print("\e[41mINTERRUPT!\e[49m");
  counter0.set(0x00000000);
}

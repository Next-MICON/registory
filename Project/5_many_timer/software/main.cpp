// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
  serial.print("Hello FPGA!\n");
  set_irq_mask(0);
  counter.set(0);
}

void loop(){
  serial.print("#");
}

void irq3(){
  serial.print("\n\e[41mINTERRUPT #3\e[49m\n");
  counter.set(0);
}

void irq4(){
  serial.print("\n\e[41mINTERRUPT #4\e[49m\n");
  counter.set(0);
}

void irq5(){
  serial.print("\n\e[41mINTERRUPT #5\e[49m\n");
  counter.set(0);
}

void irq6(){
  serial.print("\n\e[41mINTERRUPT #6\e[49m\n");
  counter.set(0);
}

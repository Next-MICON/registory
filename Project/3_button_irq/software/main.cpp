// main.cpp
#include "firmware.hpp"

void init(){
  serial0.baud(9600);
  serial0.print("\nHello\n");
  set_irq_mask(0);
}

void loop(){
  serial0.print("#");
  delayMs(500);
}

void irq5(){
  serial0.print("\e[41mINTERRUPT!\e[49m");
}

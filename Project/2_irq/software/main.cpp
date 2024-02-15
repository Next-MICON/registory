// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
  serial.print("\nHello\n");
  set_irq_mask(0);
}

void loop(){
  serial.print("#");
  delay_ms(500);
}

void irq5(){
  serial.print("\e[41mINTERRUPT!\e[49m");
}

// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
  serial.print("Hello FPGA!\n");
  set_irq_mask(0);
  counter0.set_ms(200);
  counter1.set_ms(300);
  counter2.set_ms(500);
  counter3.set_ms(700);
}

void loop(){
  serial.print("#\n");
  delay_ms(100);
}

void irq3(){
  serial.print("\e[41mIRQ#3\e[49m\n");
  counter0.set_ms(200);
}

void irq4(){
  serial.print("\e[42mIRQ#4\e[49m\n");
  counter1.set_ms(300);
}

void irq5(){
  serial.print("\e[43mIRQ#5\e[49m\n");
  counter2.set_ms(500);
}

void irq6(){
  serial.print("\e[44mIRQ#6\e[49m\n");
  counter3.set_ms(700);
}

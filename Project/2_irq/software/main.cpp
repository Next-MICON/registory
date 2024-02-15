// main.cpp
#include "firmware.hpp"

void init() {
  serial.baud(9600);
  serial.print("\nHello FPGA!\n");
  counter.set_ms(5000);
  set_irq_mask(0);
}

void loop() {
  serial.print("#");
  delay_ms(500);
}

void irq3() {
  serial.print("\n\e[41m!3\e[49m");
  delay_ms(500);
}

void irq4() {
  serial.print("\n\e[42m!4\e[49m");
  delay_ms(500);
}

void irq5() {
  serial.print("\n\e[43m!5\e[49m");
  counter.set_ms(5000);
}

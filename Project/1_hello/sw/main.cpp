// main.cpp
#include "fw.hpp"

void init() {
  serial.baud(9600);
}

void loop() {
  serial.print("Hello FPGA!\n");
  reg1.write(1);
  delay_ms(500);
  reg1.write(0);
  delay_ms(500);
}

void hoge() {}
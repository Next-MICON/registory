// main.cpp
#include "firmware.hpp"

void init(){
  serial.baud(9600);
}

void loop(){
  serial.print('a');
  if(serial.receive() == 'a') ledout.write(1);
  delay_ms(1000);
  ledout.write(0);
}

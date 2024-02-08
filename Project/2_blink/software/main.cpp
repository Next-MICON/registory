// main.cpp
#include "firmware.hpp"

void init(){
    // Code Here!
}

void loop(){
  delay_ms(500);
  ledif.write(1);
  delay_ms(500);
  ledif.write(0);
}

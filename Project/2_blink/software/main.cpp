// main.cpp
#include "firmware.hpp"

void init(){
    // Code Here!
}

void loop(){
  delayMs(500);
  ledif.write(1);
  delayMs(500);
  ledif.write(0);
}

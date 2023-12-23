#pragma once
#include <stdint.h>

class Analog {
  volatile uint32_t* reg;
public:
  Analog(volatile uint32_t* addr) : reg(addr) {}
  void duty(uint32_t val) { reg[0] = val; }
};

void analog_blink(Analog& pwm_out, uint32_t rpt = 5, uint32_t delay_us = 1000);

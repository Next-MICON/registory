#pragma once
#include <stdint.h>

class RReg {
  volatile uint32_t* reg;
public:
  RReg(volatile uint32_t* addr) : reg(addr) {}

  // Read value
  uint32_t read() { return reg[0]; }
};

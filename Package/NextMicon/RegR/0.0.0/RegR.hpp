#pragma once
#include <stdint.h>

class RegR {
  volatile uint32_t* reg;
public:
  RegR(volatile uint32_t* addr) : reg(addr) {}

  // Read value
  uint32_t read() { return reg[0]; }
};

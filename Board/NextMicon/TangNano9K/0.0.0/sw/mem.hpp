#pragma once
#include <stdint.h>

class MemRead {
  volatile uint32_t* reg;
public:
  MemRead(volatile uint32_t* addr) : reg(addr){};
  uint32_t read() { return reg[0]; };
};

class MemWrite {
  volatile uint32_t* reg;
public:
  MemWrite(volatile uint32_t* addr) : reg(addr){};
  void write(uint32_t val) { reg[0] = val; };
};

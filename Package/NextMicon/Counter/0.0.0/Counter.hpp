#pragma once
#include "cpu.hpp"
#include <stdint.h>

class Counter {
  volatile uint32_t* reg;
public:
  Counter(volatile uint32_t* addr) : reg(addr) {}
  void set(uint32_t clk) { reg[0] = clk; }
  void freq(uint32_t f) { reg[0] = CLK_FREQ / f; }
};

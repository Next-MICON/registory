#pragma once
#include "cpu.hpp"
#include <stdint.h>

class Square {
  volatile uint32_t* reg;
public:
  Square(volatile uint32_t* addr) : reg(addr) {}

  /// @brief Set oscilating clock counter
  /// @param clk
  void freq(uint32_t f) { reg[0] = CLK_FREQ / f; }

  /// @brief Stop oscilating
  void stop() { reg[0] = 0; }
};

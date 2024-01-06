#pragma once
#include <stdint.h>

class Triangle {
  volatile uint32_t* reg;
  uint32_t _clk;
public:
  Triangle(volatile uint32_t* addr) : reg(addr) {}

  /// @brief Set oscilating clock counter
  /// @param clk
  void set_clk(uint32_t clk) { _clk = clk; }

  /// @brief Stop oscilating
  void stop() { reg[0] = 0; }
};

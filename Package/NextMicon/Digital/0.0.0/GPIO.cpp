#include "Digital.hpp"
#include "cpu.hpp"

/// @brief Blink Digital in 1 Hz
/// @param gpio_out
/// @param rpt repeat time
void gpio_blink(Digital& gpio_out, uint32_t rpt, uint32_t delay_ms) {
  gpio_out.mode(Digital::Mode::OUT);
  for(int i = 0; i < rpt; ++i) {
    gpio_out.on();
    delayMs(delay_ms);
    gpio_out.off();
    delayMs(delay_ms);
  }
}

/// @brief Read gpio_in and write to gpio_out
/// @note This test require 2 Digital module
/// @param gpio_out
/// @param gpio_in
/// @param time Repeat time (depends on clock frequency)
void gpio_test(Digital& gpio_out, Digital& gpio_in, uint32_t time) {
  gpio_in.mode(Digital::Mode::IN);
  gpio_out.mode(Digital::Mode::OUT);
  for(int i = 0; i < time; ++i) {
    gpio_out.write(gpio_in.read());
  }
}

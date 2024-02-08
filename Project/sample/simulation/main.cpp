#include "firmware.hpp"

void init() {
  led.set_mode(Digital::Mode::OUT);
}

void loop() {
}

uint32_t* irq(uint32_t* regs, uint32_t irqs) {
  static uint32_t irq_counts[32] = {0};
  serial.print("\nIRQ:");
  for(uint32_t i = 0; i < 32; ++i) {
    if((irqs & (1 << i)) != 0) {
      ++irq_counts[i];
      serial.print(" #").hex(i, 2).print("*").hex(irq_counts[i], 8);
    }
  }
  serial.print("\n");
  return regs;
}

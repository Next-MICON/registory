#include "firmware.hpp"

ROM rom_cfg((volatile uint32_t*)0x0200'0000);
/* definitions */
/* end */

void main() {
  init_data();
  init_bss();
  init_array();

#ifndef SIMU
  rom_cfg.dual_io();
#endif

  init();
  for(;;) loop();
}

void __attribute__((weak)) init() {}
void __attribute__((weak)) loop() {}
uint32_t* __attribute__((weak)) irq(uint32_t* regs, uint32_t irqs) { return regs; }

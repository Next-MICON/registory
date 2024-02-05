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

uint32_t* __attribute__((weak)) irq(uint32_t* regs, uint32_t irqs) {
  if(irqs & (1 << 2)) irq2();
  if(irqs & (1 << 3)) irq3();
  if(irqs & (1 << 4)) irq4();
  if(irqs & (1 << 5)) irq5();
  if(irqs & (1 << 6)) irq6();
  if(irqs & (1 << 7)) irq7();
  if(irqs & (1 << 8)) irq8();
  if(irqs & (1 << 9)) irq9();
  if(irqs & (1 << 10)) irq10();
  if(irqs & (1 << 11)) irq11();
  if(irqs & (1 << 12)) irq12();
  if(irqs & (1 << 13)) irq13();
  if(irqs & (1 << 14)) irq14();
  if(irqs & (1 << 15)) irq15();
  return regs;
}

void __attribute__((weak)) irq2() {}
void __attribute__((weak)) irq3() {}
void __attribute__((weak)) irq4() {}
void __attribute__((weak)) irq5() {}
void __attribute__((weak)) irq6() {}
void __attribute__((weak)) irq7() {}
void __attribute__((weak)) irq8() {}
void __attribute__((weak)) irq9() {}
void __attribute__((weak)) irq10() {}
void __attribute__((weak)) irq11() {}
void __attribute__((weak)) irq12() {}
void __attribute__((weak)) irq13() {}
void __attribute__((weak)) irq14() {}
void __attribute__((weak)) irq15() {}

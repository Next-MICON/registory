#include "cpu.hpp"
#include "rom.hpp"
/* includes */
/* end */

extern ROM rom_cfg;
/* declarations */
/* end */

void init();
void loop();

extern "C" uint32_t* irq(uint32_t* regs, uint32_t irqs);

void irq2();
void irq3();
void irq4();
void irq5();
void irq6();
void irq7();
void irq8();
void irq9();
void irq10();
void irq11();
void irq12();
void irq13();
void irq14();
void irq15();

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

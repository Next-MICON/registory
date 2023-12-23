#include "cpu.hpp"
#include "rom.hpp"
/* includes */
#include "NextMicon/Serial/0.0.0/Serial.hpp"
#include "NextMicon/Digital/0.0.0/Digital.hpp"
/* end */

extern ROM rom_cfg;
/* declarations */
extern Serial serial;
extern Digital digital;
/* end */

void init();
void loop();
extern "C" uint32_t* irq(uint32_t* regs, uint32_t irqs);

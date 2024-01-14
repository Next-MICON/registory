#pragma once
#include <stdint.h>

class Digital {
  volatile uint32_t* reg;
  enum Regs {
    Reg_IOSEL = 0,
    Reg_OUT = 1,
    Reg_IN = 2
  };
  enum Mode {
    IN = 0,
    OUT = 1
  };
public:
  Digital(volatile uint32_t* addr) : reg(addr) {}

  void mode(Mode mode) { reg[Reg_IOSEL] = mode; }
  void in_mode() { reg[Reg_IOSEL] = Mode::IN; }
  void out_mode() { reg[Reg_IOSEL] = Mode::OUT; }

  uint32_t read() { return reg[Reg_IN]; }

  void write(uint32_t val) { reg[Reg_OUT] = val; }
  void on() { write(1); }
  void off() { write(0); }
};

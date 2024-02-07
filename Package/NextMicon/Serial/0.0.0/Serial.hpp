#pragma once
#include "cpu.hpp"
#include <stdint.h>

class Serial {
  volatile uint32_t* reg;
  enum Regs { Reg_Baud = 0,
              Reg_IO = 1 };
public:
  Serial(volatile uint32_t* addr) : reg(addr) {}

  void baud(uint32_t baudrate) {
    reg[0] = CLK_FREQ / baudrate;
  }

  // TX

  Serial& print(char c) {
    reg[Reg_IO] = c;
    return *this;
  }
  Serial& print(const char* str) {
    while(*str != '\0') print(*(str++));
    return *this;
  }
  Serial& hex(uint32_t num, int digits) {
    for(int i = (4 * digits) - 4; i >= 0; i -= 4)
      print("0123456789ABCDEF"[(num >> i) & 0xF]);
    return *this;
  }
  Serial& dec(uint32_t num) {
    char buffer[10];
    char* ptr = buffer;
    while(num || ptr == buffer) {
      *(ptr++) = num % 10;
      num = num / 10;
    }
    while(ptr != buffer) {
      print('0' + *(--ptr));
    }
    return *this;
  }

  // RX

  uint32_t read();
  uint32_t receive();
  uint32_t receive(uint32_t timeout);
  uint32_t read_int();
};

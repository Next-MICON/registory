#include "Serial.hpp"
#include "cpu.hpp"

uint32_t char_to_int(char c);

uint32_t Serial::read() {
  return reg[Reg_IO];
}

// Receive
// Wait RX syncronos

uint32_t Serial::receive() {
  int32_t received = -1;
  uint32_t start = rdcycle_l();
  for(;;) {
    received = read();
    if(received != -1) break;
  }
  return received;
}

// Timeout (us)
uint32_t Serial::receive(uint32_t timeout) {
  int32_t received = -1;
  uint32_t start = rdcycle(4);
  for(;;) {
    if(rdcycle(4) > timeout + start) break;
    received = read();
    if(received != -1) break;
  }
  return received;
}

uint32_t Serial::read_int() {
  uint32_t ret = 0;
  uint32_t base = 10;
  char rcv;
  while(true) {
    rcv = receive();
    if('0' <= rcv && rcv <= '9') {
      ret += ret * base + (rcv - '0');
    } else {
      break;
    }
  }
  return ret;
}

uint32_t char_to_int(char c) {
  if('0' <= c && c <= '9')
    return c - '0';
  else if('A' <= c && c <= 'F')
    return c - 'A' + 10;
  else if('a' <= c && c <= 'f')
    return c - 'a' + 10;
  else
    return -1;
}

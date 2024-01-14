#include "firmware.hpp"

auto welcome = "  _  _         _   __  __ _  \n"
               " | \\| |_____ _| |_|  \\/  (_)__ ___ _ _  \n"
               " | .` / -_) \\ /  _| |\\/| | / _/ _ \\ ' \\ \n"
               " |_|\\_\\___/_\\_\\\\__|_|  |_|_\\__\\___/_||_|\n"
               "        32bit RISC-V microcontroller\n"
               "        https://github.com/NextMicon\n\n";

auto help = "Select:\n"
            " [a] Blink LED (GPIO)\n"
            " [b] Blink LED (PWM)\n";

void init() {
  set_irq_mask(0);
  serial.baud(460800);
  serial.print(welcome);
}

void loop() {
  serial.print(help);
  switch(serial.read()) {
    case 'a': {
      serial.print("=== Blink LED (GPIO) ===\n");
      digital.out_mode();
      for(int i = 0; i < 5; ++i) {
        digital.on();
        serial.print("*");
        delayMs(500);
        digital.off();
        serial.print(".");
        delayMs(500);
      }
      serial.print("\n=== end ===\n");
    } break;
    case 'b': {
      serial.print("=== Blink LED (PWM) ===\n");
      serial.print("=== end ===\n");
    } break;
    default: {
    } break;
  }
}

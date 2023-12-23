#include "firmware.hpp"

auto welcome = "  _  _         _   __  __ _  \n"
               " | \\| |_____ _| |_|  \\/  (_)__ ___ _ _  \n"
               " | .` / -_) \\ /  _| |\\/| | / _/ _ \\ ' \\ \n"
               " |_|\\_\\___/_\\_\\\\__|_|  |_|_\\__\\___/_||_|\n"
               "        32bit RISC-V microcontroller\n"
               "        https://github.com/NextMicon\n\n";

void init() {
  set_irq_mask(0);
  serial.baud(460800);
  serial << welcome;
  digital.out_mode();
}

void loop() {
  char cmd;
  while(true) {
    serial << "Select:\n"
              " [a] Blink LED (GPIO)\n"
              " [b] Blink LED (PWM)\n";
    serial >> cmd;
    if(cmd == ' ') {
      serial << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
             << welcome;
    }
    if(cmd == 'a') {
      serial << "=== Blink LED (GPIO) ===\n";
      gpio_blink(digital);
    }
    if(cmd == 'b') {
      serial << "=== Blink LED (PWM) ===\n";
      gpio_blink(digital);
    }
  }
}

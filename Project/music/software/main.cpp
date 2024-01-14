#include "firmware.hpp"

static const uint32_t FREQ_TABLE[] = {
    0, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15,
    16, 17, 18, 19, 21, 22, 23, 25, 26, 28, 29, 31,
    33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
    65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 124,
    131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
    262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
    523, 554, 587, 622, 659, 699, 740, 784, 831, 880, 932, 988,
    1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
    2093, 2218, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
    4186, 4435, 4699, 4978, 5274, 5587, 5920, 6272, 6645, 7040, 7459, 7902,
    8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544};

void piano();

void init() {
  set_irq_mask(0);
  sampling.set(44100);
  serial.baud(460800);
  serial.print(
      "  _  _         _   __  __ _  \n"
      " | \\| |_____ _| |_|  \\/  (_)__ ___ _ _  \n"
      " | .` / -_) \\ /  _| |\\/| | / _/ _ \\ ' \\ \n"
      " |_|\\_\\___/_\\_\\\\__|_|  |_|_\\__\\___/_||_|\n"
      "        32bit RISC-V microcontroller\n"
      "        https://github.com/NextMicon\n\n");
}

void loop() {
  serial.print(
      "Select:\n"
      " [1] Blink LED\n"
      " [2] Play Sownd\n"
      " [3] Music: Rockman\n"
      " [4] Music: Ugoku\n"
      " [5] Music: Makaimura\n"
      " [6] Music: FF\n"
      " [7] Piano\n");
  switch(serial.read()) {
  case '1': {
    serial.print("=== Blink LED ===\n");
    digital.out_mode();
    for(int i = 0; i < 5; ++i) {
      digital.on();
      serial.print("*");
      delayMs(500);
      digital.off();
      serial.print(".");
      delayMs(500);
    }
    serial.print("\n=== End ===\n");
  } break;
  case '2': {
    serial.print("=== Play Sownd ===\n");
    ch1_sq.freq(FREQ_TABLE[48]);
    ch2_sq.freq(FREQ_TABLE[52]);
    ch3_sq.freq(FREQ_TABLE[55]);
    mix.set_vol(0, 0);
    mix.set_vol(1, 2);
    mix.set_vol(2, 2);
    mix.set_vol(3, 2);
    delayMs(1000);
    mix.set_vol(0, 0);
    mix.set_vol(1, 0);
    mix.set_vol(2, 0);
    mix.set_vol(3, 0);
    serial.print("=== End ===\n");
  } break;
  case '3': {
    serial.print("=== Music: Rockman ===\n");
    serial.print("  /\\_/\\ \n"
                 "6/ '-' )__ \n"
                 "(    >____|| o o \n"
                 " )  /\\ \\ \n"
                 "(__)  \\_> \n");
    serial.print("=== End ===\n");
  } break;
  case '4': {
    serial.print("=== Music: Ugoku ===\n");
    serial.print(" .. (  '-')\n");
    serial.print("=== End ===\n");
  } break;
  case '5': {
    serial.print("=== Music: Makaimura ===\n");
    serial.print("=== End ===\n");
  } break;
  case '6': {
    serial.print("=== Music: FF ===\n");
    serial.print("=== End ===\n");
  } break;
  case '7': {
    serial.print("=== Piano ===\n");
    piano();
    serial.print("=== End ===\n");
  } break;
  default: {
  } break;
  }
}

void irq4() {
  // Start Print Spectrogram
}

void piano() {
  const int N_CH = 5;
  static const char* keys = "zsxdcvgbhnjm,l.;/";
  static const char* keyboard_black = "\e[7m:\e[0m \e[7m \e[0m \e[7m \e[0m\e[7m \e[0m \e[7m \e[0m \e[7m \e[0m \e[7m \e[0m";
  static const char* keyboard_white = "\e[7m:           \e[0m";
  static const char* keyboard_black_key = "\e[7m:\e[0mS\e[7m \e[0mD\e[7m  \e[0mG\e[7m \e[0mH\e[7m \e[0mJ\e[7m \e[0m";
  static const char* keyboard_white_key = "\e[7mZ X CV B N M\e[0m";
  static const char* erase = "\r           :           :           :           :           :           :           :           :           |\r";
  static const char* tab = "           ";
  static const char* ch_name[N_CH] = {"square1", "square2", "square3", "sawtooth", "triangle"};

  const uint32_t OCT_MIN = 1;
  const uint32_t OCT_MAX = 8;

  uint32_t ch = 0;
  uint32_t octave = 3;
  uint32_t notes[N_CH] = {0};

  for(char cmd;;) {

    // Show Keyboard
    serial.print(tab);
    for(uint32_t i = OCT_MIN; i <= OCT_MAX; ++i) serial.print(i).print(i == octave ? "-----------" : "           ");
    serial.print("|\n");

    serial.print(tab);
    for(uint32_t i = OCT_MIN; i <= OCT_MAX; ++i) serial.print(keyboard_black);
    serial.print("|\n");

    serial.print(tab);
    for(uint32_t i = OCT_MIN; i <= OCT_MAX; ++i) serial.print(keyboard_white);
    serial.print("|\n");

    for(int i = 0; i < N_CH; ++i) {
      serial.print(erase);
      serial.print(i == ch ? "> " : "  ").print(ch_name[i]).print("\r");
      if(notes[i] != 0) serial.print("\e[").print(notes[i] - 1).print("C*");
      serial.print("\n");
    }
    serial.print("\e[8F");

    // --------------------------------------------------------------------------------
    // Input Command

    cmd = serial.read();

    // Exit
    if(cmd == '\n') break;
    // Change Octave
    if(OCT_MIN <= cmd - '0' && cmd - '0' <= OCT_MAX) octave = cmd - '0';
    // Change Channel
    if(cmd == 'q') ch = 0;
    if(cmd == 'w') ch = 1;
    if(cmd == 'e') ch = 2;
    if(cmd == 'r') ch = 3;
    if(cmd == 't') ch = 4;
    // Stop note
    if(cmd == ' ') {
      // player.stop(ch);
      notes[ch] = 0;
    }
    // Play sownd
    for(int i = 0; i < 17; ++i) {
      if(cmd == keys[i]) {
        int note = octave * 12 + i;
        notes[ch] = note;
        // player.play(ch, note);
      }
    }
  }
  // player.stop(0);
  // player.stop(1);
  // player.stop(2);
  // player.stop(3);
  // player.stop(4);
  serial.print("\n\n\n\n\n\n\n\n");
}

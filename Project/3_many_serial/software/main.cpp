// main.cpp
#include "aa.hpp"
#include "firmware.hpp"

const uint32_t N_USER = 5;
Serial serial[5] = {serial0, serial1, serial2, serial3, serial4};
char chat[N_USER];

const char* COLOR[6] = {"\e[41m", "\e[42m", "\e[43m", "\e[44m", "\e[45m", "\e[46m"};
const char* COLOR_CLEAR = "\e[49m";

const char* STRONG = "\e[4m";
const char* STRONG_CLEAR = "\e[0m";

void broadcast(char c) {
  for(uint32_t i = 0; i < N_USER; ++i) {
    serial[i].print(c);
  }
}

void broadcast(const char* s) {
  for(uint32_t i = 0; i < N_USER; ++i) {
    serial[i].print(s);
  }
}

void broadcast(uint32_t n) {
  for(uint32_t i = 0; i < N_USER; ++i) {
    serial[i].hex(n, 1);
  }
}

void init() {
  for(uint32_t i = 0; i < N_USER; ++i) {
    serial[i].baud(9600);
  }
  broadcast("Welcome to Serial Chat!\n");
  for(uint32_t i = 0; i < N_USER; ++i) {
    serial[i].print("You are ");
    serial[i].print(COLOR[i]);
    serial[i].print("User");
    serial[i].hex(i,1);
    serial[i].print(COLOR_CLEAR);
    serial[i].print("\n");
  }
}

void loop() {
  uint32_t cmd_cnt = 0;

  uint32_t talker = 0;
  uint32_t chat = -1;

  // 1. Find serial who wants to talk
  broadcast("> ");
  for(;;) {
    chat = serial[talker].receive(1000);
    if(chat != -1) break;
    talker = (talker + 1) % N_USER;
  }

  // 2. Broadcast talker's info
  broadcast("\b\b");
  broadcast(COLOR[talker]);
  broadcast("User");
  broadcast(talker);
  broadcast(COLOR_CLEAR);

  broadcast(" < ");
  serial[talker].print(STRONG);

  // 3. Start talking
  for(;;) {
    if(chat == -1 || chat == '\n') break;
    if(chat == '\b') broadcast("\b ");

    if(chat == cmd[cmd_cnt]) ++cmd_cnt;

    broadcast((char)chat);

    if(cmd_cnt == cmd_len) {
      cmd_cnt = 0;
      broadcast(ascii_art);
    }

    chat = serial[talker].receive(3000 * 1000);
  }

  serial[talker].print(STRONG_CLEAR);
  broadcast("\n");
}

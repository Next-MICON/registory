void init() {
  serial.baud(9600);
  serial.print("Hello FPGA!\n");
  set_irq_mask(0);
  counter.set_ms(500);
}

void loop() {
  serial.print("#");
  delay_ms(100);
}

void irq3() {
  serial.print("\n\e[41mIRQ #3\e[49m\n");
  counter.set_ms(500);
}

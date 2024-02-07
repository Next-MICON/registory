module hardware (
    // Left side
    inout pin1,
    inout pin2,
    inout pin3,
    inout pin4,
    inout pin5,
    inout pin6,
    inout pin7,
    inout pin8,
    inout pin9,
    inout pin10,
    inout pin11,
    inout pin12,
    inout pin13,

    // Right side
    inout pin14,
    inout pin15,
    inout pin16,
    inout pin17,
    inout pin18,
    inout pin19,
    inout pin20,
    inout pin21,
    inout pin22,
    inout pin23,
    inout pin24,

    // Bottom
    inout pin25,
    inout pin26,
    inout pin27,
    inout pin28,
    inout pin29,
    inout pin30,
    inout pin31,

    // SPI Flash
    inout flash_csb,
    inout flash_clk,
    inout flash_io0,
    inout flash_io1,
    inout flash_io2,
    inout flash_io3,

    // LED
    inout led,

    // USB
    inout usb_pu,
    inout usb_p,
    inout usb_n,

    // Clock
    input clk_16MHz
);

  /* iobuffer */
  In pin2_iobuf (
      .pin(pin2),
      .in(pin2_in)
  );
  wire pin2_in;
  Out pin1_iobuf (
      .pin(pin1),
      .out(serial0_tx)
  );
  Out pin3_iobuf (
      .pin(pin3),
      .out(serial1_tx)
  );
  In pin4_iobuf (
      .pin(pin4),
      .in(pin4_in)
  );
  wire pin4_in;
  Out pin5_iobuf (
      .pin(pin5),
      .out(serial2_tx)
  );
  Out pin7_iobuf (
      .pin(pin7),
      .out(serial3_tx)
  );
  In pin6_iobuf (
      .pin(pin6),
      .in(pin6_in)
  );
  wire pin6_in;
  In pin8_iobuf (
      .pin(pin8),
      .in(pin8_in)
  );
  wire pin8_in;
  Out pin9_iobuf (
      .pin(pin9),
      .out(serial4_tx)
  );
  In pin10_iobuf (
      .pin(pin10),
      .in(pin10_in)
  );
  wire pin10_in;
/* end */

  ///////////////////////////////////
  // Wire Deffinitions

  wire clk = clk_16MHz;
  wire resetn;

  ///////////////////////////////////
  // Parameters

  parameter integer MEM_WORDS = 2048;
  parameter [31:0] STACKADDR = (4 * MEM_WORDS);  // end of memory
  parameter [31:0] PROGADDR_RESET = 32'h0005_0000;  // 1 MB into flash
  parameter [31:0] PROGADDR_IRQ = 32'h0005_0010;  // 1 MB into flash

  /* parameters */
  /* end */

  ///////////////////////////////////
  // Interrupts Request

  reg [31:0] irq;
  always @* begin
    if (!resetn) irq <= 0;
    else begin
      irq = 0;
      /* irq */

/* end */
    end
  end

  ///////////////////////////////////
  // CPU

  picorv32 #(
      .STACKADDR(STACKADDR),
      .PROGADDR_RESET(PROGADDR_RESET),
      .PROGADDR_IRQ(PROGADDR_IRQ),
      .BARREL_SHIFTER(1),
      .COMPRESSED_ISA(1),
      .ENABLE_MUL(1),
      .ENABLE_DIV(1),
      .ENABLE_IRQ(1),
      .ENABLE_IRQ_QREGS(1)
  ) cpu (
      .clk      (clk),
      .resetn   (resetn),
      .mem_valid(mem_valid),
      .mem_ready(mem_ready),
      .mem_addr (mem_addr),
      .mem_wdata(mem_wdata),
      .mem_wstrb(mem_wstrb),
      .mem_rdata(mem_rdata),
      .irq      (irq)
  );

  ///////////////////////////////////
  // Memory map interface

  wire mem_valid;
  wire mem_ready;
  wire [3:0] mem_wstrb;
  wire [31:0] mem_addr;
  wire [31:0] mem_wdata;
  wire [31:0] mem_rdata;
  assign mem_ready = |{ram_ready, rom_ready, rom_cfg_ready
      /* mem_ready */
, serial0_ready, serial1_ready, serial2_ready, serial3_ready, serial4_ready
/* end */};
  assign mem_rdata = ram_ready ? ram_rdata : rom_ready ? rom_rdata : rom_cfg_ready ? rom_cfg_rdata
      /* mem_rdata */
: serial0_ready ? serial0_rdata: serial1_ready ? serial1_rdata: serial2_ready ? serial2_rdata: serial3_ready ? serial3_rdata: serial4_ready ? serial4_rdata
/* end */ : 32'b0;

  ///////////////////////////////////
  // Modules

  por por (
      .clk(clk),
      .resetn(resetn)
  );

  ram #(
      .WORDS(MEM_WORDS)
  ) ram (
      .clk(clk),
      .resetn(resetn),
      .valid(ram_valid),
      .ready(ram_ready),
      .wstrb(ram_valid ? mem_wstrb : 4'b0),
      .addr(mem_addr),
      .wdata(mem_wdata),
      .rdata(ram_rdata)
  );
  wire ram_sel = mem_addr[31:14] == 19'h0;
  wire ram_valid = mem_valid && ram_sel;
  wire ram_ready;
  wire [31:0] ram_rdata;

  spirom rom (
      .clk(clk),
      .resetn(resetn),
      .valid(rom_valid),
      .ready(rom_ready),
      .wstrb(rom_valid ? mem_wstrb : 4'b0),
      .addr (mem_addr),
      .wdata(mem_wdata),
      .rdata(rom_rdata),
      .cfg_valid(rom_cfg_valid),
      .cfg_ready(rom_cfg_ready),
      .cfg_wstrb(rom_cfg_valid ? mem_wstrb : 4'b0),
      .cfg_addr (mem_addr),
      .cfg_wdata(mem_wdata),
      .cfg_rdata(rom_cfg_rdata),
      .flash_io0_iosel(flash_io0_iosel),
      .flash_io0_in   (flash_io0_in),
      .flash_io0_out  (flash_io0_out),
      .flash_io1_iosel(flash_io1_iosel),
      .flash_io1_in   (flash_io1_in),
      .flash_io1_out  (flash_io1_out),
      .flash_io2_iosel(flash_io2_iosel),
      .flash_io2_in   (flash_io2_in),
      .flash_io2_out  (flash_io2_out),
      .flash_io3_iosel(flash_io3_iosel),
      .flash_io3_in   (flash_io3_in),
      .flash_io3_out  (flash_io3_out),
      .flash_csb(flash_csb),
      .flash_clk(flash_clk)
  );
  wire rom_sel = (mem_addr[31:20] == 12'h000) && (4'h5 <= mem_addr[19:16]);
  wire rom_valid = mem_valid && rom_sel;
  wire rom_ready;
  wire [31:0] rom_rdata;
  wire rom_cfg_sel = mem_addr[31:24] == 8'h02;
  wire rom_cfg_valid = mem_valid && rom_cfg_sel;
  wire rom_cfg_ready;
  wire [31:0] rom_cfg_rdata;
  wire flash_io0_iosel;
  wire flash_io0_in;
  wire flash_io0_out;
  wire flash_io1_iosel;
  wire flash_io1_in;
  wire flash_io1_out;
  wire flash_io2_iosel;
  wire flash_io2_in;
  wire flash_io2_out;
  wire flash_io3_iosel;
  wire flash_io3_in;
  wire flash_io3_out;
  InOut flash_io0_iobuf (
      .pin  (flash_io0),
      .iosel(flash_io0_iosel),
      .in   (flash_io0_in),
      .out  (flash_io0_out)
  );
  InOut flash_io1_iobuf (
      .pin  (flash_io1),
      .iosel(flash_io1_iosel),
      .in   (flash_io1_in),
      .out  (flash_io1_out)
  );
  InOut flash_io2_iobuf (
      .pin  (flash_io2),
      .iosel(flash_io2_iosel),
      .in   (flash_io2_in),
      .out  (flash_io2_out)
  );
  InOut flash_io3_iobuf (
      .pin  (flash_io3),
      .iosel(flash_io3_iosel),
      .in   (flash_io3_in),
      .out  (flash_io3_out)
  );

  /* instances */
  Serial serial0 (
      .clk(clk),
      .resetn(resetn),
      .valid(serial0_valid),
      .ready(serial0_ready),
      .wstrb(serial0_valid ? mem_wstrb : 4'b0),
      .addr(mem_addr),
      .wdata(mem_wdata),
      .rdata(serial0_rdata),
      .tx(serial0_tx),
      .rx(pin2_in)
  );
  wire serial0_sel = mem_addr[31:24] == 8'h3;
  wire serial0_valid = mem_valid && serial0_sel;
  wire serial0_ready;
  wire [31:0] serial0_rdata;
  wire serial0_tx;

  Serial serial1 (
      .clk(clk),
      .resetn(resetn),
      .valid(serial1_valid),
      .ready(serial1_ready),
      .wstrb(serial1_valid ? mem_wstrb : 4'b0),
      .addr(mem_addr),
      .wdata(mem_wdata),
      .rdata(serial1_rdata),
      .tx(serial1_tx),
      .rx(pin4_in)
  );
  wire serial1_sel = mem_addr[31:24] == 8'h4;
  wire serial1_valid = mem_valid && serial1_sel;
  wire serial1_ready;
  wire [31:0] serial1_rdata;
  wire serial1_tx;

  Serial serial2 (
      .clk(clk),
      .resetn(resetn),
      .valid(serial2_valid),
      .ready(serial2_ready),
      .wstrb(serial2_valid ? mem_wstrb : 4'b0),
      .addr(mem_addr),
      .wdata(mem_wdata),
      .rdata(serial2_rdata),
      .tx(serial2_tx),
      .rx(pin6_in)
  );
  wire serial2_sel = mem_addr[31:24] == 8'h5;
  wire serial2_valid = mem_valid && serial2_sel;
  wire serial2_ready;
  wire [31:0] serial2_rdata;
  wire serial2_tx;

  Serial serial3 (
      .clk(clk),
      .resetn(resetn),
      .valid(serial3_valid),
      .ready(serial3_ready),
      .wstrb(serial3_valid ? mem_wstrb : 4'b0),
      .addr(mem_addr),
      .wdata(mem_wdata),
      .rdata(serial3_rdata),
      .tx(serial3_tx),
      .rx(pin8_in)
  );
  wire serial3_sel = mem_addr[31:24] == 8'h6;
  wire serial3_valid = mem_valid && serial3_sel;
  wire serial3_ready;
  wire [31:0] serial3_rdata;
  wire serial3_tx;

  Serial serial4 (
      .clk(clk),
      .resetn(resetn),
      .valid(serial4_valid),
      .ready(serial4_ready),
      .wstrb(serial4_valid ? mem_wstrb : 4'b0),
      .addr(mem_addr),
      .wdata(mem_wdata),
      .rdata(serial4_rdata),
      .tx(serial4_tx),
      .rx(pin10_in)
  );
  wire serial4_sel = mem_addr[31:24] == 8'h7;
  wire serial4_valid = mem_valid && serial4_sel;
  wire serial4_ready;
  wire [31:0] serial4_rdata;
  wire serial4_tx;
/* end */

endmodule
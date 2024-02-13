module Reg (
    input wire clk,
    input wire resetn,

    input wire valid,
    output reg ready,
    input wire [3:0] wstrb,
    input wire [31:0] addr,
    input wire [31:0] wdata,
    output reg [31:0] rdata,

    output reg out
);

  always @(posedge clk) begin
    if (!resetn) begin
      iosel <= 0;
      out   <= 0;
    end else begin
      ready <= valid;
      (* full_case *)
      case (addr[3:2])
        2'b00: begin  // 0x...0
          rdata <= out;
          if (wstrb[0]) out <= wdata[0];
        end
      endcase
    end
  end

endmodule

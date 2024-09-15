module VVM #(
    parameter DIM = 8
) (
    input logic clk,
    input logic resetn,
    input logic [31:0] x[DIM],
    input logic [31:0] y[DIM],
    output logic [31:0] o
);

  logic [31:0] prod[DIM];
  generate
    for (genvar i = 0; i < DIM; i = i + 1) begin
      always_ff @(clk) begin
        prod[i] <= x[i] * y[i];
      end
    end
  endgenerate

endmodule

module PortIn (
    inout  pin,
    output out
);
  SB_IO #(
      .PIN_TYPE(6'b1010_01),
      .PULLUP  (1'b0)
  ) iobuf (
      .PACKAGE_PIN(pin),
      .OUTPUT_ENABLE(1'b0),
      .D_IN_0(out)
  );
endmodule

module PortInOut (
    inout  pin,
    input  iosel,
    input  in,
    output out
);
  SB_IO #(
      .PIN_TYPE(6'b1010_01),
      .PULLUP  (1'b0)
  ) iobuf (
      .PACKAGE_PIN(pin),
      .OUTPUT_ENABLE(iosel),
      .D_OUT_0(in),
      .D_IN_0(out)
  );
endmodule

module PortOut (
    inout pin,
    input in
);
  SB_IO #(
      .PIN_TYPE(6'b1010_01),
      .PULLUP  (1'b0)
  ) iobuf (
      .PACKAGE_PIN(pin),
      .OUTPUT_ENABLE(1'b1),
      .D_OUT_0(in)
  );
endmodule

module In (
    inout  pin,
    output in
);

  SB_IO #(
      .PIN_TYPE(6'b1010_01),
      .PULLUP  (1'b0)
  ) iobuf (
      .PACKAGE_PIN(pin),
      .OUTPUT_ENABLE(1'b0),
      .D_IN_0(in)
  );

endmodule

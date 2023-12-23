module Out (
    inout pin,
    input out
);

  SB_IO #(
      .PIN_TYPE(6'b1010_01),
      .PULLUP  (1'b0)
  ) iobuf (
      .PACKAGE_PIN(pin),
      .OUTPUT_ENABLE(1'b1),
      .D_OUT_0(out)
  );

endmodule

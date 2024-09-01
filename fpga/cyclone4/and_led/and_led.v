module and_led(
    input s1, s2,
    output led1);

assign led1 = s1 & s2;

endmodule
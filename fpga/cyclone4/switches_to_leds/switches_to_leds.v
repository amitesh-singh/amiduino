module switches_to_leds(input clk,
    input s1, s2, s3, s4,
    output led1, led2, led3, led4);

    debounce d1(s1, clk, led1);
    debounce d2(s2, clk, led2);
    debounce d3(s3, clk, led3);
    debounce d4(s4, clk, led4);

endmodule

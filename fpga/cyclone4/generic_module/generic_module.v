module generic_module(input clk, output led1);
blink_led #(0.2) led(.clock(clk), .led(led1));
endmodule

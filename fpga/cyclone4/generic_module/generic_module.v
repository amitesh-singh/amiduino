module generic_module(input clk, 
                      output led1, 
                      output led2,
                      output led3);
blink_led #(0.2) myboard_led1(.clock(clk), .led(led1));
blink_led #(0.5) myboard_led2(clk, led2);
blink_led #(1) myboard_led3(clk, led3);
endmodule

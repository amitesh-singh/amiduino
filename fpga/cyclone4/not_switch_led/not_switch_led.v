module not_switch_led(input switch1, output led1);
	assign led1 = ~switch1;

endmodule
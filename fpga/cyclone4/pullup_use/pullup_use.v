module pullup_use(input led1, output led2);

pullup(led2);
assign led2 = led1;

endmodule

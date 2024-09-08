module pull_up(input led1, output led2);
    
    wire led;
    assign led = led1;
    pullup(led);
    assign led2 = ~led1;

endmodule

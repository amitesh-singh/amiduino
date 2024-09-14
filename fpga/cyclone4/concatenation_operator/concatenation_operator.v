module concatenation_operator(input clk,
    input s1, s2, s3, s4,
    output led1, led2, led3, led4);

    initial begin
        led2 = 1;
        led3 = 1;
        led4 = 1;
    end
    assign {led1, led0} = 2'b01;

    assign {led4, led3} = {s1, s2};

endmodule

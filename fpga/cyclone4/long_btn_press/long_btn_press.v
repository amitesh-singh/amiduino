module long_btn_press(
    input clk,
    input s1,
    output led1,
    output led2
);

    reg [43:0] counter = 0;
    parameter CLOCK_RATE_HZ = 50_000_000;
    
    initial begin
        led1 = 1;
        led2 = 1;
    end

    always @(posedge clk) begin
        if (s1 == 0) begin 
            counter <= counter + 1;

            if (counter >= CLOCK_RATE_HZ * 5) begin ////5s
                led1 <= ~led1;
                led2 <= ~led2;
                counter <= 0;
            end 
        end else begin
            counter <= 0;
        end
    end


endmodule

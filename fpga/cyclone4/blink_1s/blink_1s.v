module blink_1s(input clk,
                output reg led1);

//remember the clock crystal frequency is 50MHz
// 50 MHz = 1/50 = 0.02 micro second
// for 1s, 1*1e6/.02 = 50_000_000;
// 2**x = 50000000
// x = log(50000000)/log(2) = 25.57 ~= 26
parameter BLINK_LED_1S_CYCLES = 50_000_000;
reg [25: 0] counter;
//initialize the counter to 0
initial counter = 0; 

always @(posedge clk) begin
    counter = counter + 1'b1;
    if (counter == BLINK_LED_1S_CYCLES)
        led1 = ~led1;
end

endmodule

//blinks led1 on every 1s

module blink_led(
    input clk,
    output led1);
    reg led1_r;
    reg [24:0] counter;
    assign led1 = led1_r;
// clk is connected to 50Mhz crystal;
// for a 50Mhz clock, you need to count to 25_000_000 to achieve a second delay
//
always @(posedge clk) begin
    counter <= counter + 1'b1;
    // In Verilog, the two notations 25_000_000 and 25000000 are functionally equivalent;
    // they both represent the same numerical value.
    // 25_000_000 makes it easier to read
    if (counter == 25_000_000) begin
        counter <= 0;
        led1_r = ~led1_r;
    end
end

//always @(counter[9]) begin
//   led1_r = !(counter[13]&counter[24]&counter[27]);
//end
endmodule
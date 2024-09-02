//blinks led1 on every?

module blink_led(
    input clk,
    output led1);
    reg led1_r;
    reg [24:0] counter;
    assign led1 = led1_r;

always @(posedge clk) begin
    counter <= counter + 1'b1;

    if (counter == 25_000_000) begin
        counter <= 0;
        led1_r = ~led1_r;
    end
end

//always @(counter[9]) begin
//   led1_r = !(counter[13]&counter[24]&counter[27]);
//end
endmodule
//blinks led1 on every?

module blink_led(
    input clk,
    output led1);
    reg led1_r;
    reg [27:0] counter;
    assign led1 = led1_r;

always @(posedge clk) begin
    counter <= counter + 1'b1;

    if (counter > 100000000) begin
        led1_r = 1'b0;
    end else begin
        led1_r = 1'b1;
    end
end

//always @(counter[9]) begin
//   led1_r = !(counter[13]&counter[24]&counter[27]);
//end
endmodule
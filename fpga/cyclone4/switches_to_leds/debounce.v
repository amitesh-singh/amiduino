// default everything is wire
// module debounce_switch(input wire s1, output wire led1);
module debounce(input s1,
    input clk,
    output led1
);

parameter CLK_CYCLES = 50_000_000; //50 MHz
parameter DEBOUNCE_TIMEOUT_CYCLES = CLK_CYCLES/100;//10ms

reg [24: 0] counter;
reg myled;
reg button_in;

initial begin
    counter <= 0;
    myled = 1;
end

always @(posedge clk) begin
    button_in <= s1;
end

always @(posedge clk) begin
    counter <= counter + 1'b1;

    if (counter > DEBOUNCE_TIMEOUT_CYCLES) begin
        myled <= button_in;
        counter <= 0;
    end
end

assign led1 = myled;

endmodule

//blinks led1 on every 1s

module blink_led(
    input clk,
    output led1);
    reg led1_r;
    // this is how we do math to calculate it
    // 2**x > 25000000
    // take log at both side
    // x log 2 = log(25000000)
    // x = log(25000000)/log 2; 
    // x = 24.57;
    // hence 25 is a good number
    reg [24:0] counter; //remember 2*25 > 25_000_000; so it can handle
    assign led1 = led1_r;

    // 50 Mhz clock, and we want 500ms delay?
    // 1/(50*1e6) = 0.02 us
    // 500*1e3/0.02 = 25_000_00;
    parameter BLINK_LED_CLOCK_CYCLES = 25_000_000;

// clk is connected to 50Mhz crystal;
// for a 50Mhz clock, you need to count to 25_000_000 to achieve a 1/2 second delay
//
always @(posedge clk) begin
    counter <= counter + 1'b1;
    // In Verilog, the two notations 25_000_000 and 25000000 are functionally equivalent;
    // they both represent the same numerical value.
    // 25_000_000 makes it easier to read
    if (counter == BLINK_LED_CLOCK_CYCLES) begin
        counter <= 0;
        //both works ~ and !
        led1_r = ~led1_r;
        //led1_r = !led1_r;
    end
end

//always @(counter[9]) begin
//   led1_r = !(counter[13]&counter[24]&counter[27]);
//end
endmodule

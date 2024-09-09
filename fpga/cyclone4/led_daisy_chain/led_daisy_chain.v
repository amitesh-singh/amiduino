module led_daisy_chain(input clk,
                       output led1, led2, led3, led4);

reg [25: 0] counter;
parameter CPU_CLK = 25_000_000;
parameter LED1_ON = 1;
parameter LED2_ON = 2;
parameter LED3_ON = 3;
parameter LED4_ON = 4;

//100ms
parameter TIMEOUT_CYCLES =  10_000_000;
reg [4: 0] led_state;
reg led_revert;
initial begin
    counter = 0;
    led_state = 0;
    led_revert = 0;
    led1 <= 1;
    led2 <= 1;
    led3 <= 1;
    led4 <= 1;
end
always @(posedge clk) begin
    counter = counter + 1'b1;
    //1s timeout
    if (counter == CPU_CLK && led_state == 0) begin
        if (led_revert == 0) led1 <= 0; //switch on the led1
        else led4 <= 0;
        counter = 0;
        led_state = LED1_ON;
    end 
    if (counter == TIMEOUT_CYCLES && led_state >= 1 && led_state <= 5) begin
        if (led_revert == 0 && led_state == LED2_ON) led2 <= 0;
        if (led_revert == 0 && led_state == LED3_ON) led3 <= 0;
        if (led_revert == 0 && led_state == LED4_ON) led4 <= 0;
        
        if (led_revert == 1 && led_state == LED2_ON) led3 <= 0;
        if (led_revert == 1 && led_state == LED3_ON) led2 <= 0;
        if (led_revert == 1 && led_state == LED4_ON) led1 <= 0;
        led_state = led_state + 1;
        counter = 0;
    end
    if (led_state >= 6) begin
        led_state = 0;
        led1 <= 1;
        led2 <= 1;
        led3 <= 1;
        led4 <= 1;
        led_revert = ~led_revert;
    end

end

endmodule

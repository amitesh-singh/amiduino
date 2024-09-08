module blink_led #(parameter N = 0.5)
                (input clock,
                 output reg led);
    parameter CLK_CYCLES = 1_000_000*50;
    reg [25: 0] counter;
    reg [25: 0] BLINK_LED_CLK_CYCLES;
    initial begin
        BLINK_LED_CLK_CYCLES = N*CLK_CYCLES;
        counter = 0;
    end

    always @(posedge clock) begin
        counter <= counter + 1'b1;
        if (counter == BLINK_LED_CLK_CYCLES) begin
            led <= ~led;
            counter <= 0;
        end
    end
endmodule
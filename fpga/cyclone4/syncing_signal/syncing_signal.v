module syncing_signal(
    input clk,
    input s1,
    output led1
);

    reg [25: 0] counter = 0;
    reg start_sync = 0;
    reg end_sync = 1;
    reg led;

    assign led = led1;

    parameter CLOCK_MHZ = 50_000_000;
    parameter LED_TIMEOUT = CLOCK_MHZ/2; //0.5s timeout
    
    always @(posedge clk ) begin
        if (end_sync) begin
            //do some stuff - task1
            counter <= counter + 1'b1;
            if (counter == LED_TIMEOUT) begin
                end_sync <= 0;
                start_sync <= 1;
                counter <= 0;
            end        
           
        end
    end

    always @(posedge clk ) begin
        if (start_sync) begin
            // do some stuff after task1
            led <= ~led;
            start_sync <= 0;
            end_sync <= 1;
        end
    end

endmodule

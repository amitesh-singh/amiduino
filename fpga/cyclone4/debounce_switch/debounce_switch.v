// default everything is wire
// module debounce_switch(input wire s1, output wire led1);
module debounce_switch(input s1,
    input clk,
    output led1
);

reg myled = 1;
initial led1 = 1;

debounce d(clk, s1, myled); //led4);

assign led1 = myled;

endmodule


module debounce(
    input clock,
    input i_switch,
    output o_switch
    //output led4
);

    parameter CLOCK_RATE_HZ = 50_000_000;
    parameter DEBOUNCE_TIMEOUT_CYCLES = CLOCK_RATE_HZ/100; //100ms

    reg [25:0] counter = 0;

    always @(posedge clock) begin
        if (i_switch == 0) begin
            counter <= counter + 1;
            //led4 <= 0;
            if (counter >= DEBOUNCE_TIMEOUT_CYCLES) begin
                o_switch <= i_switch;        
                counter <= 0;
            end
        end else begin 
            counter <= 0;
            //led4 <= 1;
            o_switch <= i_switch;
        end 
    end

endmodule

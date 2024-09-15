module uart_tx_basic(
    input clk,
    input s1,
    output tx_out,
    output led4
);

    parameter TX_CLK_CYCLE = 50_000_000/(2 * 9600);
    reg[24:0] counter = 0;
    reg [7:0] data = 8'h42; //A - we send 'A' on uart ;'0b1000001'
    reg txclk = 0;
    reg switch = 1;
    reg busy = 0;
    
    debounce d(clk, s1, switch);

    initial begin
        tx_out <= 1;
    end

    assign led4 = ~busy;

    always @(posedge clk) begin
        if (counter == TX_CLK_CYCLE) begin
            counter <= 0;
            txclk <= ~txclk;
        end else counter <= counter + 1;
    end
    
    tx_uart txuart(txclk, ~switch, data, busy, tx_out);

endmodule


module tx_uart(
    input txclk,
    input stb, //start
    input [7:0] data,
    output busy,
    output tx
);
    parameter IDLE = 3'b000;
    parameter START = 3'b001;
    parameter DATA = 3'b010;
    parameter STOP = 3'b011;


    reg [2:0] state = IDLE;
    reg [7:0] d = 0;
    reg [2:0] bit_index = 0;

    always @(posedge txclk) begin
        if (state == IDLE) begin
            busy <= 0;
            tx <= 1;  //high
            if (stb) begin
                d <= data;
                state <= START;
            end
        end else if (state == START) begin
            busy <= 1;
            tx <= 0; //start bit
            state <= DATA;
        end else if (state == DATA) begin
            if (bit_index <= 7)
                tx <= d[bit_index];
            if (&bit_index) begin
                bit_index <= 0;
                state <= STOP;
            end else
                bit_index <= bit_index + 1;
        end else if (state == STOP) begin
            busy <= 0;
            d <= 0;
            tx <= 1;
            state <= IDLE;
        end
    end


endmodule

module debounce #(parameter timeout=10)(
    input clock,
    input i_switch,
    output o_switch
    //output led4
);

    parameter CLOCK_RATE_HZ = 50_000_000;
    reg [25:0] DEBOUNCE_TIMEOUT_CYCLES = CLOCK_RATE_HZ/timeout; //100ms

    reg [25:0] counter = 0;

    always @(posedge clock) begin
        if (i_switch == 0) begin
            counter <= counter + 1;
            //led4 <= 0;
            if (counter >= DEBOUNCE_TIMEOUT_CYCLES) begin
                if (i_switch == 0)
                    o_switch <= 0;
                else
                    o_switch <= 1;
                counter <= 0;
            end
        end else begin 
            counter <= 0;
            //led4 <= 1;
            o_switch <= 1;
        end 
    end

endmodule

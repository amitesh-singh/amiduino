module uart_tx_timer(
    input clk,
    input s1,
    output tx_out,
    output led4
);

    reg [7:0] data = 8'h41; //A - we send 'A' on uart ;'0b1000001'
    reg txclk = 0;
    reg switch = 1;
    reg busy = 0;
    reg[25:0] counter = 0;

    initial begin
        tx_out <= 1;
    end

    //assign led4 = ~busy;

    always @(posedge clk) begin
        if (counter >= (50_000_000)) begin
            counter <= 0;
            switch <= 1;
        end else begin
            counter <= counter + 1;
            if (busy && switch == 1) begin
                switch <= 0;
            end
        end

    end
    tx_clk  tx_clk_gen(clk, txclk);

    tx_uart txuart(txclk, switch, data, busy, tx_out);

endmodule

module tx_clk #(crystal_clk = 50_000_000, baud_rate = 115200) (
    input reg i_clk,
    output reg o_clk
);
    reg [25: 0] TX_CLK_CYCLE = crystal_clk/(2 * baud_rate);
    reg [25: 0] counter = 0;

    always @(posedge i_clk ) begin
        if (counter >= TX_CLK_CYCLE) begin
            counter <= 0;
            o_clk = ~o_clk;
        end else
            counter <= counter + 1;    
    end
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

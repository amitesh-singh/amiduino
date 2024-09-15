module uart_tx_string(
    input clk,
    input s1,
    output tx_out,
    output led4
);

    parameter TX_CLK_CYCLE = 50_000_000/(2 * 9600);
    reg[24:0] counter = 0;
    reg [7:0] data = 0;
    reg txclk = 0;
    reg switch;
    reg busy = 0;

    reg [7:0] string_data[0:11];
    reg[3:0] char_index = 0;
    reg read_next = 0;

    initial begin
        // Load "Hello World" into string_data (ASCII values)
        string_data[0] = 8'h48;  // 'H'
        string_data[1] = 8'h65;  // 'e'
        string_data[2] = 8'h6C;  // 'l'
        string_data[3] = 8'h6C;  // 'l'
        string_data[4] = 8'h6F;  // 'o'
        string_data[5] = 8'h20;  // ' ' (space)
        string_data[6] = 8'h57;  // 'W'
        string_data[7] = 8'h6F;  // 'o'
        string_data[8] = 8'h72;  // 'r'
        string_data[9] = 8'h6C;  // 'l'
        string_data[10] = 8'h64; // 'd'
        string_data[11] = 8'h00; // NULL terminator to indicate end
    end
    
    //assign switch = ~s1;
    debounce d(s1, clk, switch);

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

    // Transmit characters one by one
    always @(posedge txclk) begin
        if (~busy && switch && read_next && string_data[char_index] != 8'h00) begin
            data <= string_data[char_index];
            char_index <= char_index + 1;
        end
        if (char_index >= 8) char_index <= 0;
    end

    tx_uart txuart(txclk, ~switch, data, read_next, busy, tx_out);

endmodule


module tx_uart(
    input txclk,
    input stb, //start
    input [7:0] data,
    output read_next,
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
            read_next <= 1;
            if (stb && ~busy) begin
                d <= data;
                state <= START;
                read_next <= 0;
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

module debounce(input sw,
    input clk,
    output reg myled
);

parameter CLK_CYCLES = 50_000_000; //50 MHz
parameter DEBOUNCE_TIMEOUT_CYCLES = CLK_CYCLES/10;//100ms

reg [26: 0] counter;
//reg myled;
reg button_in;

initial begin
    counter <= 0;
    myled = 1;
end

always @(posedge clk) begin
    button_in <= sw;
end

always @(posedge clk) begin
    counter <= counter + 1'b1;

    if (counter > DEBOUNCE_TIMEOUT_CYCLES) begin
        myled <= button_in;
        counter <= 0;
    end
end

endmodule

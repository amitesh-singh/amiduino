module beep(
    input clk,
    output beep);
    
    reg beep_status;
    reg [27:0] counter;
    assign beep = beep_status;

always @(posedge clk) begin
    counter <= counter + 1'b1;
end
always @(counter[9]) begin
   beep_status = !(counter[13]&counter[24]&counter[27]);
end
endmodule
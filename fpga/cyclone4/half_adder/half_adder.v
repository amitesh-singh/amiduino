// half_adder with two one-bit inputs; 
// led[0] - sum, led[1] - carry
module half_adder(
	input s1, 
	input s2, 
	output [1:0] led);
	
	reg sum;
	reg carry;
	initial begin
		led = 2'b11;
	end
	assign led[0] = ~sum;
	assign led[1] = ~carry;
// we could write below two lines as well
// assign sum = ~s1 ^ ~s2;
// assign carry = ~s1 & ~s2;
// or
always @(*) begin
	sum = ~s1 ^ ~s2;
	carry = ~s1 & ~s2;
end

endmodule
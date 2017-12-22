`timescale 1ns / 1ps
module test;

reg x;
reg y;

wire z;

comp uut (
    .x(x),
    .y(y),
    .z(z)
);

initial begin
  x = 0;
  y = 0;

  #20 x = 1;
  #20 y = 1;
  #20 y = 0;
  #20 x = 1;
  #40; x = 1; y = 1; x= ~y;
end

initial begin
  $monitor("x=%d,y=%d,z=%d \n",x,y,z);
end
endmodule 
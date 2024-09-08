// Copyright (C) 2024  Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions 
// and other software and tools, and any partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License 
// Subscription Agreement, the Intel Quartus Prime License Agreement,
// the Intel FPGA IP License Agreement, or other applicable license
// agreement, including, without limitation, that your use is for
// the sole purpose of programming logic devices manufactured by
// Intel and sold by Intel or its authorized distributors.  Please
// refer to the applicable agreement for further details, at
// https://fpgasoftware.intel.com/eula.

module pull_up
(
// {ALTERA_ARGS_BEGIN} DO NOT REMOVE THIS LINE!

	led2,
	s4,
	s3,
	s2,
	s1,
	led1,
	led3,
	led4,
	beep,
	clk
// {ALTERA_ARGS_END} DO NOT REMOVE THIS LINE!

);

// {ALTERA_IO_BEGIN} DO NOT REMOVE THIS LINE!
output			led2;
input			s4;
input			s3;
input			s2;
input			s1;
input			led1;
input			led3;
input			led4;
input			beep;
input			clk;

// {ALTERA_IO_END} DO NOT REMOVE THIS LINE!
// {ALTERA_MODULE_BEGIN} DO NOT REMOVE THIS LINE!
// {ALTERA_MODULE_END} DO NOT REMOVE THIS LINE!
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2016/09/28 22:51:40
// Design Name: 
// Module Name: dac_read_en_sig_generator
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module dac_read_en_sig_generator( clk, gpio_in, read_en_out );
input clk;
input gpio_in;
output read_en_out;

wire vcc, gnd;

assign vcc = 1;
assign gnd = 0;
wire d1_q, d2_q;
assign read_en_out = d2_q;

FDCE #(.INIT(1'b0)) dff_1(
   .D(vcc),       // 1-bit Data input
   .C(gpio_in),   // 1-bit Clock input
   .CLR(d2_q),    // 1-bit Asynchronous clear input
   .CE(vcc),      // 1-bit Clock enable input
   .Q(d1_q)       // 1-bit Data output 
);

FDCE #(.INIT(1'b0)) dff_2(
   .D(d1_q),       // 1-bit Data input
   .C(clk),        // 1-bit Clock input
   .CLR(gnd),      // 1-bit Asynchronous clear input
   .CE(vcc),       // 1-bit Clock enable input
   .Q(d2_q)        // 1-bit Data output 
);

endmodule
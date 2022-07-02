//2022.6.28 xuxin
`include "defines.v"

module ysyx_22040931_IF(
    input wire reset,
    input wire clock,
    input wire mux_pc,
    input wire [`ysyx_22040931_PC_BUS] branch,
    
    output wire fetch_enb,
    output wire [`ysyx_22040931_PC_BUS]   if_pc
    //output wire [`ysyx_22040931_INST_BUS] instr

);

wire [`ysyx_22040931_PC_BUS] pc_i;
assign pc_i = mux_pc ? branch : if_pc + 4;


ysyx_22040931_PC ysyx_22040931_PC(
  
  .reset(reset),
  .clock(clock),
  .pc_i(pc_i),

  .pc_o(if_pc),
  .fetch_enb(fetch_enb)
  
);

endmodule

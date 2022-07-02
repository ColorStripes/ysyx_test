//6.27 xuxin
`include "defines.v"

module ysyx_22040931_Btype(
    input wire [6 : 0] opcode,
    input wire [2 : 0] opcode_3,
    input wire [`ysyx_22040931_DATA_BUS] r_data1,
    input wire [`ysyx_22040931_DATA_BUS] r_data2,

    output wire             jump,
    output wire [4 : 0]    aluop,
    output wire [2 : 0]     exop,
    output wire            btype
);


    wire [9 : 0] chose = {opcode_3, opcode};
    wire [9 : 0] out;
    wire [5 : 0] jumpb;



    assign jumpb[5] = (r_data1 == r_data2) ? 1'b1 : 1'b0;
    assign jumpb[4] = (r_data1[63] != r_data2[63]) ? ((r_data1[63] < r_data2[63]) ? 1'b1 : 1'b0)
                   : ((r_data1[62 : 0] >= r_data2[62 : 0]) ? 1'b1 : 1'b0);
    assign jumpb[3] = (r_data1 >= r_data2) ? 1'b1 : 1'b0;//
    assign jumpb[2] = (r_data1[63] != r_data2[63]) ? ((r_data1[63] > r_data2[63]) ? 1'b1 : 1'b0)
                   : ((r_data1[62 : 0] < r_data2[62 : 0]) ? 1'b1 : 1'b0);
    assign jumpb[1] = (r_data1 < r_data2) ? 1'b1 : 1'b0;//
    assign jumpb[0] = (r_data1 != r_data2) ? 1'b1 : 1'b0;

    ysyx_22040931_MuxD #(6, 10, 10) Btype (out, chose, 10'b0000_0000_0000_00, {
    `ysyx_22040931_beq,  {1'b1,`ysyx_22040931_No,`ysyx_22040931_NO, jumpb[5]},
    `ysyx_22040931_bge,  {1'b1,`ysyx_22040931_No,`ysyx_22040931_NO, jumpb[4]},
    `ysyx_22040931_bgeu, {1'b1,`ysyx_22040931_No,`ysyx_22040931_NO, jumpb[3]},
    `ysyx_22040931_blt,  {1'b1,`ysyx_22040931_No,`ysyx_22040931_NO, jumpb[2]},
    `ysyx_22040931_bltu, {1'b1,`ysyx_22040931_No,`ysyx_22040931_NO, jumpb[1]},
    `ysyx_22040931_bne,  {1'b1,`ysyx_22040931_No,`ysyx_22040931_NO, jumpb[0]}
    });


    assign jump = out[0];
    assign aluop = out[5 : 1];
    assign exop = out[8 : 6];
    assign btype = out[9];


endmodule

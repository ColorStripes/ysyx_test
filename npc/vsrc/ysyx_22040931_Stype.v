//6.25 xuxin
`include "defines.v"

module ysyx_22040931_Stype(
    input wire [6 : 0] opcode,
    input wire [2 : 0] opcode_3,

    output wire [2 : 0]    memwop,
    output wire [4 : 0]    aluop,
    output wire [2 : 0]     exop,
    output wire            stype
);


    wire [11 : 0] out;
    wire [9 : 0] chose = {opcode_3, opcode};

    ysyx_22040931_MuxD #(4, 10, 12) Stype (out, chose, 12'b0000_0000_00000, {
    `ysyx_22040931_sd,  {1'b1,`ysyx_22040931_Stort,`ysyx_22040931_SORT,`ysyx_22040931_W_EIG},
    `ysyx_22040931_sw,  {1'b1,`ysyx_22040931_Stort,`ysyx_22040931_SORT,`ysyx_22040931_W_FOR},
    `ysyx_22040931_sh,  {1'b1,`ysyx_22040931_Stort,`ysyx_22040931_SORT,`ysyx_22040931_W_DOU},
    `ysyx_22040931_sb,  {1'b1,`ysyx_22040931_Stort,`ysyx_22040931_SORT,`ysyx_22040931_W_ONE}
    });


    assign memwop = out[2 : 0];
    assign aluop = out[7 : 3];
    assign exop = out[10 : 8];
    assign stype = out[11];

endmodule

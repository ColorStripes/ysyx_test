//6.28 xuxin
`include "defines.v"

module ysyx_22040931_Utype(
    input wire [6 : 0] opcode,

    output wire [4 : 0]    aluop,
    output wire [2 : 0]     exop,
    output wire            utype
);

    wire [6 : 0] chose = opcode;
    wire [8 : 0] out;

    ysyx_22040931_MuxD #(2, 7, 9) Utype (out, chose, 9'b0000_0000_00, {
    `ysyx_22040931_auipc,  {1'b1,`ysyx_22040931_Arith,`ysyx_22040931_PC},
    `ysyx_22040931_lui,    {1'b1,`ysyx_22040931_LUI  ,`ysyx_22040931_NO}
    });

    assign aluop = out[4 : 0];
    assign exop = out[7 : 5];
    assign utype = out[8];

endmodule

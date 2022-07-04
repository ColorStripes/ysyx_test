//2022.6.28 xuxin
`include "defines.v"


module ysyx_22040931_EX(

    input wire w_ena_i,
    input wire [4 : 0] w_addr_i,
    input wire [`ysyx_22040931_PC_BUS] pc_i,
    
    input wire [`ysyx_22040931_DATA_BUS] data1,
    input wire [`ysyx_22040931_DATA_BUS] data2,
    input wire [`ysyx_22040931_DATA_BUS] imm,

    input wire [2 : 0]     exop,
    input wire [4 : 0]    aluop,
    //mem    
    input wire [2 : 0]   memwop_i,
    input wire [2 : 0]   memrop_i,
    input wire          mem_ena_i,
    input wire           mem_wr_i,
    
    //regfile
    output wire w_ena,
    output wire [4 : 0] w_addr,
    output wire [`ysyx_22040931_DATA_BUS] w_data,
    //mem
    output wire [2 : 0]   memwop,
    output wire [2 : 0]   memrop,
    output wire          mem_ena,
    output wire           mem_wr,    
    output wire [`ysyx_22040931_MEM_BUS] mem_addr,
    output wire [`ysyx_22040931_DATA_BUS] mem_data,
    //liushuixian
    output wire [`ysyx_22040931_PC_BUS] pc_o

);

    assign pc_o = pc_i;
    assign w_ena = w_ena_i;
    assign w_addr = w_addr_i;
    assign mem_data = data2;

    assign memwop = memwop_i;
    assign memrop = memrop_i;
    assign mem_ena = mem_ena_i;
    assign mem_wr = mem_wr_i;

    wire [`ysyx_22040931_DATA_BUS]result;
    ysyx_22040931_ALU ysyx_22040931_ALU(
    .num1(data1),
    .num2(data2),
    .imm(imm),
    .pc(pc_i),
    .op(aluop),

    .out(result)
    );


    ysyx_22040931_MuxD #(3, 3, 64)  w_data_mux (
        w_data,
        exop,
        `ysyx_22040931_ZERO_NUM,
        {   
            `ysyx_22040931_Arith,  result, 
            `ysyx_22040931_Short,  {{32{result[31]}}, result[31 : 0]},
            `ysyx_22040931_LUI,    imm
        }
    );

    ysyx_22040931_MuxD #(2, 3, `ysyx_22040931_MEM)  mem_addr_mux (
        mem_addr,
        exop,
        `ysyx_22040931_ZERO_PC,
        {   
            `ysyx_22040931_Stort,   result[`ysyx_22040931_MEM_BUS],
            `ysyx_22040931_LOAD,    result[`ysyx_22040931_MEM_BUS]
        }
    );
    
endmodule

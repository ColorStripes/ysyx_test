//2022.6.24 xuxin
`include "defines.v"

module ysyx_22040931_ID(

    //regfile
    input wire reset,
    input wire clock,
    input wire w_ena_i,
    input wire [4 : 0] w_addr_i,
    input wire [`ysyx_22040931_DATA_BUS] w_data_i,
    //liushuixian
    input wire [`ysyx_22040931_PC_BUS] pc_i,
    input wire [`ysyx_22040931_INST_BUS] instr,

    //liushuixian
    output wire [`ysyx_22040931_PC_BUS] pc_o,    
    //branch
    output wire [`ysyx_22040931_PC_BUS] branch,      //////////////////////////////////
    output wire mux_pc,
    //regfile
    output wire 		    w_ena,
    output wire [4 : 0]    w_addr,
    output wire [`ysyx_22040931_DATA_BUS] data1,
    output wire [`ysyx_22040931_DATA_BUS] data2,
    //ex
    output wire mem_ena,
    output wire mem_wr,
    output wire [`ysyx_22040931_DATA_BUS] imm,
    output wire [2 : 0]     exop,
    output wire [4 : 0]    aluop,    
    output wire [2 : 0]   memwop,
    output wire [2 : 0]   memrop

);


assign pc_o = pc_i;

    wire [2 : 0]     ztype;
    wire 		    r_ena1;
    wire [4 : 0]   r_addr1;
    wire 		    r_ena2;
    wire [4 : 0]   r_addr2;
    wire [`ysyx_22040931_DATA_BUS] r_data2;

    wire [`ysyx_22040931_PC_BUS] ibranch;
    wire [`ysyx_22040931_PC_BUS] bbranch;
    wire [`ysyx_22040931_PC_BUS] jbranch;
    assign ibranch = data1 + imm;
    assign bbranch = pc_i + imm;
    assign jbranch = pc_i + imm;

    ysyx_22040931_MuxD #(3, 3, 64)  branch_mux (
        branch,
        ztype,
        `ysyx_22040931_ZERO_PC,
        {
           `ysyx_22040931_It, ibranch,
           `ysyx_22040931_Bt, bbranch,
           `ysyx_22040931_Jt, jbranch
        }
    );

    ysyx_22040931_Decoder ysyx_22040931_Decoder(

    .instr(instr),
    .r_data1(data1),
    .r_data2(r_data2),
	.w_ena(w_ena),
	.w_addr(w_addr),
    .r_ena1(r_ena1),
    .r_addr1(r_addr1),
    .r_ena2(r_ena2),
    .r_addr2(r_addr2),
    .mem_ena(mem_ena),
    .mem_wr(mem_wr),
    .ztype(ztype),

    .exop(exop),
    .aluop(aluop),    
    .memwop(memwop),
    .memrop(memrop),
    .jump(mux_pc)

    );

    ysyx_22040931_IMM ysyx_22040931_IMM(
    .instr(instr[31 : 7]),
    .opt(ztype),

    .imm(imm)
    );


    ysyx_22040931_Regfile ysyx_22040931_Regfile(
	.reset(reset),
    	.clock(clock),
	.w_ena(w_ena_i),
	.w_addr(w_addr_i),
	.w_data(w_data_i),
	
	.r_ena1(r_ena1),
	.r_addr1(r_addr1),
	.r_data1(data1),      //OUT1
	
	.r_ena2(r_ena2),
	.r_addr2(r_addr2),
	.r_data2(r_data2)     //OUT2

    );
    
    assign data2 = r_ena2 ? r_data2 : imm;



endmodule

//2022.6.28 xuxin
`include "defines.v"

module top(
    input wire reset,
    input wire clock,
    input wire [`ysyx_22040931_INST_BUS] instr,
    //mem
    input wire [`ysyx_22040931_DATA_BUS] momory_data,
    
    output wire fetch_enb,
    output wire [`ysyx_22040931_PC_BUS] pc,
    //mem
    output wire [1 : 0]       memop,
    output wire             mem_ena,
    output wire              mem_wr,
    output wire [`ysyx_22040931_MEM_BUS] mem_addr,
    output wire [`ysyx_22040931_DATA_BUS] mem_stor_data

);

//DPI-C
import "DPI-C" function void Inst(input int instruct);
always @(posedge clock) begin
	if(reset == 1'b0) begin
	   Inst(instr);
	end
end

import "DPI-C" function void E(input int a);
always @(instr) begin
	if(instr == 32'b0000000_00001_00000_000_00000_11100_11) begin
		Inst(instr);
		E(1);
	end
	else begin
		E(0);
	end
end

wire [63:0] rdata;
import "DPI-C" function void mem_read(input longint raddr, output longint rdata);
import "DPI-C" function void mem_write(input longint waddr, input longint wdata, input byte wmask);
wire [7 : 0] wmask;
assign wmask = (memop == `ysyx_22040931_SIZE_D) ? 8'b1111_1111 :
               (memop == `ysyx_22040931_SIZE_W) ? (mem_addr[2] ? 8'b1111_0000 : 8'b0000_1111) :
               (memop == `ysyx_22040931_SIZE_H) ? (8'b0000_0011 << (mem_addr[2 : 0] << 1))    :
               (memop == `ysyx_22040931_SIZE_B) ? (8'b0000_0001 <<  mem_addr[2 : 0] )         : 8'b0000_0000;

always @(*) begin
      	mem_read(mem_addr, rdata);
      	mem_write(mem_addr, mem_stor_data, wmask);
end

ysyx_22040931_IF ysyx_22040931_IF(
    .reset(reset),
    .clock(clock),
    .mux_pc(mux_pc),
    .branch(branch),
    
    .fetch_enb(fetch_enb),
    .if_pc(pc)

);

wire mux_pc;
wire [`ysyx_22040931_PC_BUS] branch;


ysyx_22040931_ID ysyx_22040931_ID(

    //regfile
    .reset(reset),
    .clock(clock),
    .w_ena_i(wb_w_ena),
    .w_addr_i(wb_w_addr),
    .w_data_i(wb_w_data),
    //liushuixian
    .pc_i(pc),
    .instr(instr),

    //liushuixian
    .pc_o(id_pc),       
    //branch
    .branch(branch),
    .mux_pc(mux_pc),
    //regfile
    .w_ena(id_w_ena),
    .w_addr(id_w_addr),
    .data1(data1),
    .data2(data2),
    //ex
    .mem_ena(id_mem_ena),
    .mem_wr(id_mem_wr),
    .imm(imm),
    .exop(exop),
    .aluop(aluop),    
    .memwop(id_memwop),
    .memrop(id_memrop)

);

wire [`ysyx_22040931_PC_BUS] id_pc;
wire 		    id_w_ena;
wire [4 : 0]    id_w_addr;
wire [`ysyx_22040931_DATA_BUS] data1;
wire [`ysyx_22040931_DATA_BUS] data2;
//ex
wire id_mem_ena;
wire id_mem_wr;
wire [`ysyx_22040931_DATA_BUS] imm;
wire [2 : 0]     exop;
wire [4 : 0]    aluop;   
wire [2 : 0]   id_memwop;
wire [2 : 0]   id_memrop;


ysyx_22040931_EX ysyx_22040931_EX(
    //regfile
    .w_ena_i(id_w_ena),
    .w_addr_i(id_w_addr),
    .pc_i(id_pc),
    
    .data1(data1),/////////
    .data2(data2),
    .imm(imm),

    .exop(exop),
    .aluop(aluop),
    //mem    
    .memwop_i(id_memwop),
    .memrop_i(id_memrop),
    .mem_ena_i(id_mem_ena),
    .mem_wr_i(id_mem_wr),
    
    //regfile
    .w_ena(ex_w_ena),
    .w_addr(ex_w_addr),
    .w_data(ex_w_data),
    //mem
    .memwop(memwop),
    .memrop(memrop),
    .mem_ena(ex_mem_ena),
    .mem_wr(ex_mem_wr),    
    .mem_addr(ex_mem_addr),
    .mem_data(ex_mem_data),
    //liushuixian
    .pc_o(ex_pc)

);

wire ex_w_ena;
wire [4 : 0] ex_w_addr;
wire [`ysyx_22040931_DATA_BUS] ex_w_data;
//mem
wire [2 : 0]      memwop;
wire [2 : 0]      memrop;
wire          ex_mem_ena;
wire           ex_mem_wr;    
wire [`ysyx_22040931_MEM_BUS] ex_mem_addr;
wire [`ysyx_22040931_DATA_BUS] ex_mem_data;
//liushuixian
wire [`ysyx_22040931_PC_BUS] ex_pc;


ysyx_22040931_MEM ysyx_22040931_MEM(

    .w_ena_i(ex_w_ena),
    .w_addr_i(ex_w_addr),
    .w_data_i(ex_w_data),
    //mem
    .memwop(memwop),
    .memrop(memrop),
    .mem_ena_i(ex_mem_ena),
    .mem_wr_i(ex_mem_wr),
    .mem_addr_i(ex_mem_addr),
    .mem_stor_data_i(ex_mem_data),
    .mem_data(rdata), ////////////////////////////////////////////
    
    //liushuixian
    .pc_i(ex_pc),


    .w_ena(mem_w_ena),
    .w_addr(mem_w_addr),
    .w_data(mem_w_data),
    //mem
    .memop(memop),                                    //
    .mem_ena(mem_ena),                                //
    .mem_wr(mem_wr),                                  //
    .mem_addr(mem_addr),                             //
    .mem_stor_data(mem_stor_data),                   //
    //liushuixian
    .pc_o(mem_pc)

);

wire mem_w_ena;
wire [4 : 0] mem_w_addr;
wire [`ysyx_22040931_DATA_BUS] mem_w_data;

//liushuixian
wire [`ysyx_22040931_PC_BUS] mem_pc;


ysyx_22040931_WB ysyx_22040931_WB(

    .w_ena_i(mem_w_ena),
    .w_addr_i(mem_w_addr),
    .w_data_i(mem_w_data),
    //liushuixian
    .pc_i(mem_pc),
    

    .w_ena(wb_w_ena),
    .w_addr(wb_w_addr),
    .w_data(wb_w_data)
);

wire wb_w_ena;
wire [4 : 0] wb_w_addr;
wire [`ysyx_22040931_DATA_BUS] wb_w_data;

endmodule

//2022.6.24 xuxin
`include "defines.v"


module ysyx_22040931_ALU(
    input wire [`ysyx_22040931_DATA_BUS] num1,
    input wire [`ysyx_22040931_DATA_BUS] num2,
    input wire [`ysyx_22040931_DATA_BUS] imm,
    input wire [`ysyx_22040931_PC_BUS]    pc,
    input wire [4 : 0] op,

    output reg [`ysyx_22040931_DATA_BUS] out
);

wire signed [63 : 0] num1_s;
wire signed [31 : 0] num1_sw;
wire signed [63 : 0] num2_s;
wire signed [31 : 0] num2_sw;

assign num1_s = num1;
assign num2_s = num2;
assign num1_sw = num1[31 : 0];
assign num2_sw = num2[31 : 0];



    always @(*) begin
        case (op)
            `ysyx_22040931_ADD: begin
                out = num1 + num2;

            end
            `ysyx_22040931_SUB: begin
                out = num1 - num2;

            end
            `ysyx_22040931_AND: begin
                out = num1 & num2;
  
            end
            `ysyx_22040931_OR: begin
                out = num1 | num2;

            end
            `ysyx_22040931_XOR: begin
                out = num1 ^ num2;
    
            end
            `ysyx_22040931_COM: begin

                if(num1[63] == num2[63]) begin
                    if(num1[62 :0] < num2[62 :0]) begin
                        out = 1;
                    end
                    else begin
                        out = 0;
                    end
                end
                else begin
                    if(num1[63] > num2[63]) begin
                        out = 1;
                    end
                    else begin
                        out = 0;
                    end
                end
            end
            `ysyx_22040931_COMU: begin

                if(num1 < num2) begin
                    out = 1;
                end
                else begin
                    out = 0;
                end
            end
            `ysyx_22040931_SHIL:begin
                out = num1 << num2[5 : 0];
 
            end
            `ysyx_22040931_SHILW:begin
                out = num1 << num2[4 : 0];

            end
            `ysyx_22040931_SHIR:begin
                out = num1 >> num2[5 : 0];

            end
            `ysyx_22040931_SHIRW:begin
                out = {{32{1'b0}} , {num1[31 : 0] >> num2[4 : 0]}};
   
            end
            `ysyx_22040931_SRA:begin
                out =  num1_s >>> num2[5 : 0];

            end    
            `ysyx_22040931_SRAW :begin
                out =  {{32{num1_sw[31]}} , {num1_sw >>> num2[4 : 0]}};

            end   
            `ysyx_22040931_PC:begin
                out = num2 + pc;
      
            end
            `ysyx_22040931_JUMP:begin
                out = pc + 4;
  
            end
            `ysyx_22040931_SORT:begin
                out = num1 + imm;
  
            end
            `ysyx_22040931_REMW:begin
                out = {32'b0, num1_sw % num2_sw};
            end
            `ysyx_22040931_REMUW:begin
                out = {32'b0, num1[31 : 0] % num2[31 : 0]};
            end
            `ysyx_22040931_REMU:begin
                out = num1 % num2;
            end
            `ysyx_22040931_REM:begin
                out = num1_s % num2_s;
            end
            `ysyx_22040931_MUL:begin
                out = num1 * num2;
            end
            `ysyx_22040931_MULH:begin
                out = (num1_s * num2_s) >> 64;
            end
            `ysyx_22040931_MULHSU:begin
                out = (num1_s * num2) >> 64;
            end
            `ysyx_22040931_MULHU:begin
                out = (num1 * num2) >> 64;
            end
            `ysyx_22040931_DIV:begin
                out = num1_s / num2_s;
            end   
            `ysyx_22040931_DIVU:begin
                out = num1 / num2;
            end
            `ysyx_22040931_DIVUW:begin
                out = {32'b0, num1_sw / num2_sw};
            end

            default: begin
                out = 64'h00000000_00000000;
            end
        endcase
    end
endmodule

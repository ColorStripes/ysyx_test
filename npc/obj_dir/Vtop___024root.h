// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class Vtop__Syms;
class Vtop_VerilatedVcd;


//----------

VL_MODULE(Vtop___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_OUT8(fetch_enb,0,0);
    VL_OUT8(memop,1,0);
    VL_OUT8(mem_ena,0,0);
    VL_OUT8(mem_wr,0,0);
    VL_IN(instr,31,0);
    VL_IN64(momory_data,63,0);
    VL_OUT64(pc,63,0);
    VL_OUT64(mem_addr,63,0);
    VL_OUT64(mem_stor_data,63,0);

    // LOCAL SIGNALS
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ top__DOT__mux_pc;
        CData/*2:0*/ top__DOT__ysyx_22040931_ID__DOT__ztype;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__branch_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__opt_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__jump_mux__DOT__i1__DOT__lut_out;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__jump_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__Rtype__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype1__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype2__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype3__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype4__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__Stype__DOT__i1__DOT__hit;
        CData/*5:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__jumpb;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__Btype__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Jtype__DOT__Jtype__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Utype__DOT__Utype__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_IMM__DOT__imm_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_EX__DOT__w_data_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_EX__DOT__mem_addr_mux__DOT__i1__DOT__hit;
        CData/*2:0*/ top__DOT__ysyx_22040931_MEM__DOT__memwrop;
        CData/*1:0*/ top__DOT__ysyx_22040931_MEM__DOT__memop_mux__DOT__i1__DOT__lut_out;
        CData/*0:0*/ top__DOT__ysyx_22040931_MEM__DOT__memop_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_r_data_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data1_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data2_mux__DOT__i1__DOT__hit;
        CData/*0:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data3_mux__DOT__i1__DOT__hit;
        SData/*10:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__opt_mux__DOT__i1__DOT__lut_out;
        SData/*8:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__out;
        SData/*8:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__Rtype__DOT__i1__DOT__lut_out;
        SData/*12:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__out;
        SData/*12:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__out1;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__out2;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__out3;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__out4;
        SData/*15:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__chose2;
        SData/*12:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype__DOT__i1__DOT__lut_out;
        SData/*12:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype1__DOT__i1__DOT__lut_out;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype2__DOT__i1__DOT__lut_out;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype3__DOT__i1__DOT__lut_out;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype4__DOT__i1__DOT__lut_out;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__out;
        SData/*9:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__chose;
        SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__Stype__DOT__i1__DOT__lut_out;
        SData/*9:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__chose;
        SData/*9:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__out;
        SData/*9:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__Btype__DOT__i1__DOT__lut_out;
        SData/*9:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Jtype__DOT__out;
        SData/*9:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Jtype__DOT__Jtype__DOT__i1__DOT__lut_out;
        SData/*8:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Utype__DOT__out;
        SData/*8:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Utype__DOT__Utype__DOT__i1__DOT__lut_out;
        IData/*16:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__chose;
        IData/*16:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__chose3;
        IData/*17:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__chose4;
        IData/*31:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Regfile__DOT__i;
        QData/*63:0*/ top__DOT__rdata;
        QData/*63:0*/ top__DOT__branch;
        QData/*63:0*/ top__DOT__data1;
        QData/*63:0*/ top__DOT__data2;
        QData/*63:0*/ top__DOT__imm;
        QData/*63:0*/ top__DOT__ex_w_data;
        QData/*63:0*/ top__DOT__ex_mem_addr;
        QData/*63:0*/ top__DOT__ysyx_22040931_IF__DOT__pc_i;
    };
    struct {
        QData/*63:0*/ top__DOT__ysyx_22040931_ID__DOT__r_data2;
        QData/*63:0*/ top__DOT__ysyx_22040931_ID__DOT__branch_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_IMM__DOT__imm_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_EX__DOT__result;
        QData/*63:0*/ top__DOT__ysyx_22040931_EX__DOT__w_data_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_EX__DOT__mem_addr_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_r_data;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__stor_data_one;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__stor_data_two;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__stor_data_for;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_r_data_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data1_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data2_mux__DOT__i1__DOT__lut_out;
        QData/*63:0*/ top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data3_mux__DOT__i1__DOT__lut_out;
        VlUnpacked<VlWide<3>/*66:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__branch_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__branch_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__branch_mux__DOT__i1__DOT__data_list;
        VlUnpacked<SData/*13:0*/, 6> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__opt_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 6> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__opt_mux__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*10:0*/, 6> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__opt_mux__DOT__i1__DOT__data_list;
        VlUnpacked<CData/*3:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__jump_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__jump_mux__DOT__i1__DOT__key_list;
        VlUnpacked<CData/*0:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__jump_mux__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*25:0*/, 28> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__Rtype__DOT__i1__DOT__pair_list;
        VlUnpacked<IData/*16:0*/, 28> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__Rtype__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*8:0*/, 28> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Rtype__DOT__Rtype__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*16:0*/, 4> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*3:0*/, 4> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*12:0*/, 4> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*22:0*/, 13> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype1__DOT__i1__DOT__pair_list;
        VlUnpacked<SData/*9:0*/, 13> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype1__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*12:0*/, 13> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype1__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*27:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype2__DOT__i1__DOT__pair_list;
        VlUnpacked<SData/*15:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype2__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*11:0*/, 3> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype2__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*28:0*/, 5> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype3__DOT__i1__DOT__pair_list;
        VlUnpacked<IData/*16:0*/, 5> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype3__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*11:0*/, 5> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype3__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*29:0*/, 2> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype4__DOT__i1__DOT__pair_list;
        VlUnpacked<IData/*17:0*/, 2> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype4__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*11:0*/, 2> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT__Itype4__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*21:0*/, 4> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__Stype__DOT__i1__DOT__pair_list;
        VlUnpacked<SData/*9:0*/, 4> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__Stype__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*11:0*/, 4> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Stype__DOT__Stype__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*19:0*/, 6> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__Btype__DOT__i1__DOT__pair_list;
        VlUnpacked<SData/*9:0*/, 6> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__Btype__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*9:0*/, 6> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT__Btype__DOT__i1__DOT__data_list;
        VlUnpacked<IData/*16:0*/, 1> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Jtype__DOT__Jtype__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*6:0*/, 1> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Jtype__DOT__Jtype__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*9:0*/, 1> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Jtype__DOT__Jtype__DOT__i1__DOT__data_list;
        VlUnpacked<SData/*15:0*/, 2> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Utype__DOT__Utype__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*6:0*/, 2> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Utype__DOT__Utype__DOT__i1__DOT__key_list;
        VlUnpacked<SData/*8:0*/, 2> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Utype__DOT__Utype__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*66:0*/, 5> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_IMM__DOT__imm_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 5> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_IMM__DOT__imm_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 5> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_IMM__DOT__imm_mux__DOT__i1__DOT__data_list;
        VlUnpacked<QData/*63:0*/, 32> top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Regfile__DOT__regs;
        VlUnpacked<VlWide<3>/*66:0*/, 3> top__DOT__ysyx_22040931_EX__DOT__w_data_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 3> top__DOT__ysyx_22040931_EX__DOT__w_data_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 3> top__DOT__ysyx_22040931_EX__DOT__w_data_mux__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*66:0*/, 2> top__DOT__ysyx_22040931_EX__DOT__mem_addr_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 2> top__DOT__ysyx_22040931_EX__DOT__mem_addr_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 2> top__DOT__ysyx_22040931_EX__DOT__mem_addr_mux__DOT__i1__DOT__data_list;
    };
    struct {
        VlUnpacked<CData/*4:0*/, 7> top__DOT__ysyx_22040931_MEM__DOT__memop_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 7> top__DOT__ysyx_22040931_MEM__DOT__memop_mux__DOT__i1__DOT__key_list;
        VlUnpacked<CData/*1:0*/, 7> top__DOT__ysyx_22040931_MEM__DOT__memop_mux__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*66:0*/, 7> top__DOT__ysyx_22040931_MEM__DOT__mem_r_data_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 7> top__DOT__ysyx_22040931_MEM__DOT__mem_r_data_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 7> top__DOT__ysyx_22040931_MEM__DOT__mem_r_data_mux__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*66:0*/, 4> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*2:0*/, 4> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 4> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data_mux__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*69:0*/, 8> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data1_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*5:0*/, 8> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data1_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 8> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data1_mux__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*68:0*/, 4> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data2_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*4:0*/, 4> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data2_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 4> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data2_mux__DOT__i1__DOT__data_list;
        VlUnpacked<VlWide<3>/*67:0*/, 2> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data3_mux__DOT__i1__DOT__pair_list;
        VlUnpacked<CData/*3:0*/, 2> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data3_mux__DOT__i1__DOT__key_list;
        VlUnpacked<QData/*63:0*/, 2> top__DOT__ysyx_22040931_MEM__DOT__mem_stor_data3_mux__DOT__i1__DOT__data_list;
    };

    // LOCAL VARIABLES
    CData/*3:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT____Vcellinp__Itype____pinNumber2;
    CData/*5:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data1_mux____pinNumber2;
    CData/*4:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data2_mux____pinNumber2;
    CData/*3:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data3_mux____pinNumber2;
    CData/*0:0*/ __Vclklast__TOP__clock;
    SData/*10:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT____Vcellout__opt_mux____pinNumber1;
    SData/*11:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT____Vcellinp__jump_mux____pinNumber4;
    VlWide<7>/*200:0*/ top__DOT__ysyx_22040931_ID__DOT____Vcellinp__branch_mux____pinNumber4;
    VlWide<3>/*83:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT____Vcellinp__opt_mux____pinNumber4;
    VlWide<3>/*67:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Itype__DOT____Vcellinp__Itype____pinNumber4;
    VlWide<4>/*119:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_Decoder__DOT__ysyx_22040931_Btype__DOT____Vcellinp__Btype____pinNumber4;
    VlWide<11>/*334:0*/ top__DOT__ysyx_22040931_ID__DOT__ysyx_22040931_IMM__DOT____Vcellinp__imm_mux____pinNumber4;
    VlWide<7>/*200:0*/ top__DOT__ysyx_22040931_EX__DOT____Vcellinp__w_data_mux____pinNumber4;
    VlWide<5>/*133:0*/ top__DOT__ysyx_22040931_EX__DOT____Vcellinp__mem_addr_mux____pinNumber4;
    VlWide<15>/*468:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_r_data_mux____pinNumber4;
    VlWide<9>/*267:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data_mux____pinNumber4;
    VlWide<18>/*559:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data1_mux____pinNumber4;
    VlWide<9>/*275:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data2_mux____pinNumber4;
    VlWide<5>/*135:0*/ top__DOT__ysyx_22040931_MEM__DOT____Vcellinp__mem_stor_data3_mux____pinNumber4;
    QData/*63:0*/ __Vtask_top__DOT__mem_read__4__rdata;
    VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;

    // INTERNAL VARIABLES
    Vtop__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vtop___024root);  ///< Copying not allowed
  public:
    Vtop___024root(const char* name);
    ~Vtop___024root();

    // INTERNAL METHODS
    void __Vconfigure(Vtop__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard

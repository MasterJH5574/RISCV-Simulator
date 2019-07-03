#ifndef PPCA_RISC_V_INSTRUCTIONS_H
#define PPCA_RISC_V_INSTRUCTIONS_H

#include "global.h"
#include <map>
#include <string>

enum ins_name {
    NAME0,
    ADD, SLT, SLTU, AND, OR, XOR, SLL, SRL, SUB, SRA,                                    // R
    JALR, LB, LH, LW, LBU, LHU, ADDI, SLTI, SLTIU, ANDI, ORI, XORI, SLLI, SRLI, SRAI,    // I
    SB, SH, SW,                                                                          // S
    BEQ, BNE, BLT, BGE, BLTU, BGEU,                                                      // B
    LUI, AUIPC,                                                                          // U
    JAL,                                                                                 // J
};

/*
 * 1 ADD     11 JALR    21 ORI      31 BLT
 * 2 SLT     12 LB      22 XORI     32 BGE
 * 3 SLTU    13 LH      23 SLLI     33 BLTU
 * 4 AND     14 LW      24 SRLI     34 BGEU
 * 5 OR      15 LBU     25 SRAI     35 LUI
 * 6 XOR     16 LHU     26 SB       36 AUIPC
 * 7 SLL     17 ADDI    27 SH       37 JAL
 * 8 SRL     18 SLTI    28 SW
 * 9 SUB     19 SLTIU   29 BEQ
 * 10 SRA    20 ANDI    30 BNE
 */

class instruction {
public:
    ins_name name;

    int32 imm;
    int32 funct3, funct7;
    int32 rs1, rs2, rd;
    int32 opcode;
    int32 str;

private:
    static int32 get_I_imm(const int32 &str);
    static int32 get_S_imm(const int32 &str);
    static int32 get_B_imm(const int32 &str);
    static int32 get_U_imm(const int32 &str);
    static int32 get_J_imm(const int32 &str);

public:
    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();

public:
    instruction();
    explicit instruction(int32 str);

public:
    void fake_EX();
    void fake_MEM();

public:
    void show_ins();
};

#endif //PPCA_RISC_V_INSTRUCTIONS_H

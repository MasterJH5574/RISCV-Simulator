#ifndef PPCA_RISC_V_INSTRUCTIONS_H
#define PPCA_RISC_V_INSTRUCTIONS_H

#include "global.h"

enum ins_name {
    NAME0,
    ADD, SLT, SLTU, AND, OR, XOR, SLL, SRL, SUB, SRA,                                    // R
    JALR, LB, LH, LW, LBU, LHU, ADDI, SLTI, SLTIU, ANDI, ORI, XORI, SLLI, SRLI, SRAI,    // I
    SB, SH, SW,                                                                          // S
    BEQ, BNE, BLT, BGE, BLTU, BGEU,                                                      // B
    LUI, AUIPC,                                                                          // U
    JAL,                                                                                 // J
};

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
    void show_ins();
};

#endif //PPCA_RISC_V_INSTRUCTIONS_H

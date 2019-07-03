#include <cstring>
#include "stages.h"

IF_ID::IF_ID() {
    empty = true;
    _pc = 0u;
    ins_str = 0u;
}

ID_EX::ID_EX() {
    empty = true;
    _pc = 0u;
    imm = 0u;
    v_rs1 = v_rs2 = 0u;
}

EX_MEM::EX_MEM() {
    empty = true;
    _pc = 0u;
    imm = 0u;
    v_rs1 = v_rd = 0u;
}

MEM_WB::MEM_WB() {
    empty = true;
    _pc = 0u;
    v_rd = 0u;
}

void IF_ID::clear() {
    empty = true;
    _pc = 0u;
    ins_str = 0u;
}

void ID_EX::clear() {
    empty = true;
    _pc = 0u;
    imm = 0u;
    v_rs1 = v_rs2 = 0u;
}

void EX_MEM::clear() {
    empty = true;
    _pc = 0u;
    imm = 0u;
    v_rs1 = v_rd = 0u;
}

void MEM_WB::clear() {
    empty = true;
    _pc = 0u;
    v_rd = 0u;
}

void IF_ID::push() {                     // procedure IF
    empty = false;
    _pc = pc;
    memcpy(&ins_str, mem + _pc, sizeof(int32));
    pc += 4;
}

void IF_ID::execute(ID_EX &id_ex) {      // procedure ID
    if (ins_str == 0x00c68223) {
        id_ex.clear();
        return;
    }

    instruction ins(ins_str);
    int32 imm = 0u;
    int32 v_rs1 = 0u, v_rs2 = 0u;

    switch (ins.name) {
        case LUI: case AUIPC: case JAL:
            imm = ins.imm; break;

        case JALR: v_rs1 = reg[ins.rs1], imm = ins.imm; break;

        case BEQ: case BNE: case BLT: case BGE: case BLTU: case BGEU:
        case SB: case SH: case SW:
            v_rs1 = reg[ins.rs1], v_rs2 = reg[ins.rs2];
            imm = ins.imm;
            break;

        case LB: case LH: case LW: case LBU: case LHU:
        case ADDI: case SLTI: case SLTIU:
        case XORI: case ORI: case ANDI:
        case SLLI: case SRLI: case SRAI:
            v_rs1 = reg[ins.rs1], imm = ins.imm;
            break;

        case ADD: case SUB:
        case SLL: case SRL: case SRA:
        case SLT: case SLTU:
        case XOR: case OR: case AND:
            v_rs1 = reg[ins.rs1], v_rs2 = reg[ins.rs2];
            break;

        default:
            break;
    }

    id_ex.empty = false;
    id_ex._pc = _pc;
    id_ex.ins = ins;
    id_ex.v_rs1 = v_rs1, id_ex.v_rs2 = v_rs2;
    id_ex.imm = imm;

    clear();
}

void ID_EX::execute(EX_MEM &ex_mem) {    // procedure EX
    int32 v_rd = 0u;

    switch (ins.name) {
        case JAL: v_rd = _pc + 4, pc = _pc + imm; break;
        case JALR: v_rd = _pc + 4, pc = (imm + v_rs1) & (-2u); break;

        case BEQ: pc = _pc + (v_rs1 == v_rs2 ? imm : 4); break;
        case BNE: pc = _pc + (v_rs1 != v_rs2 ? imm : 4); break;
        case BLT: pc = _pc + ((int)v_rs1 < (int)v_rs2 ? imm : 4); break;
        case BGE: pc = _pc + ((int)v_rs1 >= (int)v_rs2 ? imm : 4); break;
        case BLTU: pc = _pc + (v_rs1 < v_rs2 ? imm : 4); break;
        case BGEU: pc = _pc + (v_rs1 >= v_rs2 ? imm : 4); break;

        case LUI: v_rd = imm; break;
        case AUIPC: v_rd += imm; break;

        case ADDI: v_rd = v_rs1 + imm; break;
        case SLTI: v_rd = (int)v_rs1 < (int)imm; break;
        case SLTIU: v_rd = v_rs1 < imm; break;
        case XORI: v_rd = v_rs1 ^ imm; break;
        case ORI: v_rd = v_rs1 | imm; break;
        case ANDI: v_rd = v_rs1 & imm; break;

        case SLLI: v_rd = v_rs1 << (imm & 31u); break;
        case SRLI: v_rd = v_rs1 >> (imm & 31u); break;
        case SRAI: v_rd = (int)(v_rs1) >> (imm & 31u); break;

        case ADD: v_rd = v_rs1 + v_rs2; break;
        case SUB: v_rd = v_rs1 - v_rs2; break;

        case SLL: v_rd = v_rs1 << (v_rs2 & 31u); break;
        case SRL: v_rd = v_rs1 >> (v_rs2 & 31u); break;
        case SRA: v_rd = (int)(v_rs1) >> (v_rs2 & 31u); break;

        case SLT: v_rd = (int)v_rs1 < (int)v_rs2; break;
        case SLTU: v_rd = v_rs1 < v_rs2; break;

        case XOR: v_rd = v_rs1 ^ v_rs2; break;
        case OR: v_rd = v_rs1 | v_rs2; break;
        case AND: v_rd = v_rs1 & v_rs2; break;

        case SB: case SH: case SW: // for store, v_rd = v_rs2 = the value of reg[rs2] to be stored in memory
            v_rd = v_rs2; break;

        default:
            break;
    }

    ex_mem.empty = false;
    ex_mem._pc = _pc;
    ex_mem.ins = ins;
    ex_mem.imm = imm;
    ex_mem.v_rs1 = v_rs1, ex_mem.v_rd = v_rd;

    clear();
}

void EX_MEM::execute(MEM_WB &mem_wb) {   // procedure MEM
    switch (ins.name) {
        case LB:
            char res_LB;
            memcpy(&res_LB, mem + (v_rs1 + imm), sizeof(char));
            v_rd = (int32)res_LB;
            break;
        case LH:
            short res_LH;
            memcpy(&res_LH, mem + (v_rs1 + imm), sizeof(short));
            v_rd = (int32)res_LH;
            break;
        case LW:
            memcpy(&v_rd, mem + (v_rs1 + imm), sizeof(int32));
            break;
        case LBU:
            uchar res_LBU;
            memcpy(&res_LBU, mem + (v_rs1 + imm), sizeof(uchar));
            v_rd = (int32)res_LBU;
            break;
        case LHU:
            unsigned short res_LHU;
            memcpy(&res_LHU, mem + (v_rs1 + imm), sizeof(unsigned short));
            v_rd = (int32)res_LHU;
            break;

        case SB:
            char res_SB;
            res_SB = (char)v_rd;
            memcpy(mem + (v_rs1 + imm), &res_SB, sizeof(char));
            break;
        case SH:
            short res_SH;
            res_SH = (short)v_rd;
            memcpy(mem + (v_rs1 + imm), &res_SH, sizeof(short));
            break;
        case SW:
            memcpy(mem + (v_rs1 + imm), &v_rd, sizeof(int32));
            break;

        default:
            break;
    }

    mem_wb.empty = false;
    mem_wb.v_rd = v_rd;
    mem_wb.ins = ins;
    mem_wb._pc = _pc;

    clear();
}

void MEM_WB::execute() {                 // procedure WB
    switch (ins.name) {
        case BEQ: case BNE: case BLT: case BGE: case BLTU: case BGEU:
        case SB: case SH: case SW:
            break;

        default:
            reg[ins.rd] = v_rd;
            break;
    }

    reg[0] = 0;
    clear();
}
#include <iostream>
#include <cstring>
#include <bitset>
#include "stages.h"

inline int min(const int &a, const int &b) {
    return a < b ? a : b;
}

inline int max(const int &a, const int &b) {
    return a > b ? a : b;
}

void IF_ID::display() {
    printf("IF/ID:\n");
    std::cout << "empty = " << empty << std::endl;
    printf("pc of IF/ID = %x\n", _pc);
    printf("ins_str = %x\n", ins_str);
}

void ID_EX::display() {
    printf("ID/EX:\n");
    std::cout << "empty = " << empty << std::endl;
    printf("pc of ID_EX = %x\n", _pc);
    printf("name = %d\n", name);
}

void EX_MEM::display() {
    printf("EX/MEM:\n");
    std::cout << "empty = " << empty << std::endl;
    printf("name = %d\n", name);
    printf("time = %d\n", time);
}

void MEM_WB::display() {
    printf("MEM/WB:\n");
    std::cout << "empty = " << empty << std::endl;
    printf("name = %d\n", name);
}

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
    name = NAME0;
    rd = 0u;
}

EX_MEM::EX_MEM() {
    empty = true;
    imm = 0u;
    v_rs1 = v_rd = 0u;
    name = NAME0;
    rd = 0u;

    time = 0;
}

MEM_WB::MEM_WB() {
    empty = true;
    v_rd = 0u;
    name = NAME0;
    rd = 0u;
}

void IF_ID::push() {                     // procedure IF
    empty = false;
    _pc = pc;
    memcpy(&ins_str, mem + _pc, sizeof(int32));
    pc += 4;
}

void IF_ID::execute(ID_EX *id_ex, EX_MEM *ex_mem, MEM_WB *mem_wb) {      // procedure ID
    if (ins_str == 0x00c68223) {
        id_ex->empty = true;
        return;
    }

    instruction ins(ins_str);
    int32 imm = 0u;
    int32 v_rs1 = 0u, v_rs2 = 0u;

    switch (ins.name) {
        case LUI: case AUIPC: case JAL:
            imm = ins.imm; break;

        case JALR:
            if (!used[ins.rs1]) {
                v_rs1 = reg[ins.rs1], imm = ins.imm;
                break;
            } else if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && ex_mem->rd == ins.rs1) {
                v_rs1 = ex_mem->v_rd, imm = ins.imm;
                break;
            } else if (!mem_wb->empty && mem_wb->rd == ins.rs1) {
                v_rs1 = mem_wb->v_rd, imm = ins.imm;
                break;
            } else
                return;

        case BEQ: case BNE: case BLT: case BGE: case BLTU: case BGEU:
        case SB: case SH: case SW:
            if (!used[ins.rs1] && !used[ins.rs2]) {
                v_rs1 = reg[ins.rs1], v_rs2 = reg[ins.rs2];
                imm = ins.imm;
                break;
            } else if (!used[ins.rs1]) {
                if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && ex_mem->rd == ins.rs2) {
                    v_rs1 = reg[ins.rs1], v_rs2 = ex_mem->v_rd;
                    imm = ins.imm;
                    break;
                } else if (!mem_wb->empty && mem_wb->rd == ins.rs2) {
                    v_rs1 = reg[ins.rs1], v_rs2 = mem_wb->v_rd;
                    imm = ins.imm;
                    break;
                } else
                    return;
            } else if (!used[ins.rs2]) {
                if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && ex_mem->rd == ins.rs1) {
                    v_rs1 = ex_mem->v_rd, v_rs2 = reg[ins.rs2];
                    imm = ins.imm;
                    break;
                } else if (!mem_wb->empty && mem_wb->rd == ins.rs1) {
                    v_rs1 = mem_wb->v_rd, v_rs2 = reg[ins.rs2];
                    imm = ins.imm;
                    break;
                } else
                    return;
            } else if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && !mem_wb->empty
                        && ex_mem->rd == ins.rs1 && mem_wb->rd == ins.rs2) {
                v_rs1 = ex_mem->v_rd, v_rs2 = mem_wb->v_rd;
                imm = ins.imm;
                break;
            } else if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && !mem_wb->empty
                        && mem_wb->rd == ins.rs1 && ex_mem->rd == ins.rs2) {
                v_rs1 = mem_wb->v_rd, v_rs2 = ex_mem->v_rd;
                imm = ins.imm;
                break;
            } else
                return;

        case LB: case LH: case LW: case LBU: case LHU:
        case ADDI: case SLTI: case SLTIU:
        case XORI: case ORI: case ANDI:
        case SLLI: case SRLI: case SRAI:
            if (!used[ins.rs1]) {
                v_rs1 = reg[ins.rs1], imm = ins.imm;
                break;
            } else if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && ex_mem->rd == ins.rs1) {
                v_rs1 = ex_mem->v_rd, imm = ins.imm;
                break;
            } else if (!mem_wb->empty && mem_wb->rd == ins.rs1) {
                v_rs1 = mem_wb->v_rd, imm = ins.imm;
                break;
            } else
                return;

        case ADD: case SUB:
        case SLL: case SRL: case SRA:
        case SLT: case SLTU:
        case XOR: case OR: case AND:
            if (!used[ins.rs1] && !used[ins.rs2]) {
                v_rs1 = reg[ins.rs1], v_rs2 = reg[ins.rs2];
                break;
            } else if (!used[ins.rs1]) {
                if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && ex_mem->rd == ins.rs2) {
                    v_rs1 = reg[ins.rs1], v_rs2 = ex_mem->v_rd;
                    break;
                } else if (!mem_wb->empty && mem_wb->rd == ins.rs2) {
                    v_rs1 = reg[ins.rs1], v_rs2 = mem_wb->v_rd;
                    break;
                } else
                    return;
            } else if (!used[ins.rs2]) {
                if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && ex_mem->rd == ins.rs1) {
                    v_rs1 = ex_mem->v_rd, v_rs2 = reg[ins.rs2];
                    break;
                } else if (!mem_wb->empty && mem_wb->rd == ins.rs1) {
                    v_rs1 = mem_wb->v_rd, v_rs2 = reg[ins.rs2];
                    break;
                } else
                    return;
            } else if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && !mem_wb->empty
                        && ex_mem->rd == ins.rs1 && mem_wb->rd == ins.rs2) {
                v_rs1 = ex_mem->v_rd, v_rs2 = mem_wb->v_rd;
                break;
            } else if ((ex_mem->name <= 11 || ex_mem->name >= 17) && !ex_mem->empty && !mem_wb->empty
                        && mem_wb->rd == ins.rs1 && ex_mem->rd == ins.rs2) {
                v_rs1 = mem_wb->v_rd, v_rs2 = ex_mem->v_rd;
                break;
            } else
                return;

        default:
            break;
    }

    if (ins.rs1 == 0)
        v_rs1 = 0;
    if (ins.rs2 == 0)
        v_rs2 = 0;

    id_ex->empty = false;
    id_ex->_pc = _pc;
    id_ex->v_rs1 = v_rs1, id_ex->v_rs2 = v_rs2;
    id_ex->rd = ins.rd;
    id_ex->imm = imm;
    id_ex->name = ins.name;

    empty = true;

    if (ins.name <= 25 || ins.name >= 35) {
        used[ins.rd]++;
        used[0] = 0;
    }

    // branch prediction
    if (ins.name >= 29 && ins.name <= 34) {
        int32 id = (_pc >> 2u) & 31u;
        int state = pred[id].cnt[pred[id].history];
        if (state & 2) // assume taken, pc changes
            pc = _pc + ins.imm;
        // else assume not taken, pc doesn't change
    }
}

void ID_EX::execute(EX_MEM *ex_mem, IF_ID *if_id) {    // procedure EX
    int32 v_rd = 0u;
    int32 id = (_pc >> 2u) & 31u;
    int history = pred[id].history;
    int &state = pred[id].cnt[history];

    switch (name) {
        case JAL: v_rd = _pc + 4, pc = _pc + imm, if_id->empty = true; break;
        case JALR: v_rd = _pc + 4, pc = (imm + v_rs1) & (-2u), if_id->empty = true; break;

        /* without branch prediction
        case BEQ: pc = _pc + (v_rs1 == v_rs2 ? imm : 4), if_id->empty = true; break;
        case BNE: pc = _pc + (v_rs1 != v_rs2 ? imm : 4), if_id->empty = true; break;
        case BLT: pc = _pc + ((int)v_rs1 < (int)v_rs2 ? imm : 4), if_id->empty = true; break;
        case BGE: pc = _pc + ((int)v_rs1 >= (int)v_rs2 ? imm : 4), if_id->empty = true; break;
        case BLTU: pc = _pc + (v_rs1 < v_rs2 ? imm : 4), if_id->empty = true; break;
        case BGEU: pc = _pc + (v_rs1 >= v_rs2 ? imm : 4), if_id->empty = true; break;
        */

        // with branch prediction
        case BEQ:
            if (v_rs1 == v_rs2) {       // taken
                if (!(state & 2)) {     // wrong
                    pc = _pc + imm, if_id->empty = true;
                    state++;
                } else                  // correct
                    state = min(state + 1, 3);
                pred[id].history = (pred[id].history << 1 | 1) & 3;
            } else {                    // not taken
                if (state & 2) {        // wrong
                    pc = _pc + 4, if_id->empty = true;
                    state--;
                } else
                    state = max(state - 1, 0);
                pred[id].history = (pred[id].history << 1) & 3;
            }
            break;
        case BNE:
            if (v_rs1 != v_rs2) {       // taken
                if (!(state & 2)) {     // wrong
                    pc = _pc + imm, if_id->empty = true;
                    state++;
                } else                  // correct
                    state = min(state + 1, 3);
                pred[id].history = (pred[id].history << 1 | 1) & 3;
            } else {                    // not taken
                if (state & 2) {        // wrong
                    pc = _pc + 4, if_id->empty = true;
                    state--;
                } else
                    state = max(state - 1, 0);
                pred[id].history = (pred[id].history << 1) & 3;
            }
            break;
        case BLT:
            if ((int)v_rs1 < (int)v_rs2) {       // taken
                if (!(state & 2)) {     // wrong
                    pc = _pc + imm, if_id->empty = true;
                    state++;
                } else                  // correct
                    state = min(state + 1, 3);
                pred[id].history = (pred[id].history << 1 | 1) & 3;
            } else {                    // not taken
                if (state & 2) {        // wrong
                    pc = _pc + 4, if_id->empty = true;
                    state--;
                } else
                    state = max(state - 1, 0);
                pred[id].history = (pred[id].history << 1) & 3;
            }
            break;
        case BGE:
            if ((int)v_rs1 >= (int)v_rs2) {       // taken
                if (!(state & 2)) {     // wrong
                    pc = _pc + imm, if_id->empty = true;
                    state++;
                } else                  // correct
                    state = min(state + 1, 3);
                pred[id].history = (pred[id].history << 1 | 1) & 3;
            } else {                    // not taken
                if (state & 2) {        // wrong
                    pc = _pc + 4, if_id->empty = true;
                    state--;
                } else
                    state = max(state - 1, 0);
                pred[id].history = (pred[id].history << 1) & 3;
            }
            break;
        case BLTU:
            if (v_rs1 < v_rs2) {       // taken
                if (!(state & 2)) {     // wrong
                    pc = _pc + imm, if_id->empty = true;
                    state++;
                } else                  // correct
                    state = min(state + 1, 3);
                pred[id].history = (pred[id].history << 1 | 1) & 3;
            } else {                    // not taken
                if (state & 2) {        // wrong
                    pc = _pc + 4, if_id->empty = true;
                    state--;
                } else
                    state = max(state - 1, 0);
                pred[id].history = (pred[id].history << 1) & 3;
            }
            break;
        case BGEU:
            if (v_rs1 >= v_rs2) {       // taken
                if (!(state & 2)) {     // wrong
                    pc = _pc + imm, if_id->empty = true;
                    state++;
                } else                  // correct
                    state = min(state + 1, 3);
                pred[id].history = (pred[id].history << 1 | 1) & 3;
            } else {                    // not taken
                if (state & 2) {        // wrong
                    pc = _pc + 4, if_id->empty = true;
                    state--;
                } else
                    state = max(state - 1, 0);
                pred[id].history = (pred[id].history << 1) & 3;
            }
            break;

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

    ex_mem->empty = false;
    ex_mem->imm = imm;
    ex_mem->v_rs1 = v_rs1, ex_mem->v_rd = v_rd;
    ex_mem->rd = rd;
    ex_mem->name = name;
    ex_mem->time = 0;

    if (!ex_mem->empty && ex_mem->name >= 26 && ex_mem->name <= 34)
        ex_mem->rd = 32u;

    empty = true;
}

void EX_MEM::execute(MEM_WB *mem_wb) {   // procedure MEM
    if (name == LB || name == LH || name == LW || name == LBU
            || name == LHU || name == SB || name == SH || name == SW) {
        if (time == 3) {
            switch (name) {
                case LB:
                    char res_LB;
                    memcpy(&res_LB, mem + (v_rs1 + imm), sizeof(char));
                    v_rd = (int32) res_LB;
                    break;
                case LH:
                    short res_LH;
                    memcpy(&res_LH, mem + (v_rs1 + imm), sizeof(short));
                    v_rd = (int32) res_LH;
                    break;
                case LW:
                    memcpy(&v_rd, mem + (v_rs1 + imm), sizeof(int32));
                    break;
                case LBU:
                    uchar res_LBU;
                    memcpy(&res_LBU, mem + (v_rs1 + imm), sizeof(uchar));
                    v_rd = (int32) res_LBU;
                    break;
                case LHU:
                    unsigned short res_LHU;
                    memcpy(&res_LHU, mem + (v_rs1 + imm), sizeof(unsigned short));
                    v_rd = (int32) res_LHU;
                    break;

                case SB:
                    char res_SB;
                    res_SB = (char) v_rd;
                    memcpy(mem + (v_rs1 + imm), &res_SB, sizeof(char));
                    break;
                case SH:
                    short res_SH;
                    res_SH = (short) v_rd;
                    memcpy(mem + (v_rs1 + imm), &res_SH, sizeof(short));
                    break;
                case SW:
                    memcpy(mem + (v_rs1 + imm), &v_rd, sizeof(int32));
                    break;

                default:
                    break;
            }
            mem_wb->empty = false;
            mem_wb->v_rd = v_rd;
            mem_wb->rd = rd;
            mem_wb->name = name;
            empty = true;
        } else
            time++;
    } else {
        mem_wb->empty = false;
        mem_wb->v_rd = v_rd;
        mem_wb->rd = rd;
        mem_wb->name = name;
        empty = true;
    }

    if (!mem_wb->empty && mem_wb->name >= 26 && mem_wb->name <= 34)
        mem_wb->rd = 32u;
}

void MEM_WB::execute() {                 // procedure WB
    switch (name) {
        case BEQ: case BNE: case BLT: case BGE: case BLTU: case BGEU:
        case SB: case SH: case SW:
            break;

        default:
            used[rd]--;
            used[0] = 0;
            reg[rd] = v_rd;
            break;
    }

    reg[0] = 0;
    empty = true;
}
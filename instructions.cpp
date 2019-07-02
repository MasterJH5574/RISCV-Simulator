#include <iostream>
#include <cstring>
#include <bitset>
#include "instructions.h"
#include "global.h"

int32 instruction::get_I_imm(const int32 &str) {
    int32 res = 0u;
    if ((str >> 31u) == 1u)
        res |= 0xfffff800;
    res |= (str >> 20u) & 2047u;
    return res;
}

int32 instruction::get_S_imm(const int32 &str) {
    int32 res = 0u;
    if ((str >> 31u) == 1u)
        res |= 0xfffff800;
    res |= ((str >> 25u) & 63u) << 5u;
    res |= ((str >> 8u) & 15u) << 1u;
    res |= (str >> 7u) & 1u;
    return res;
}

int32 instruction::get_B_imm(const int32 &str) {
    int32 res = 0u;
    if ((str >> 31u) == 1u)
        res |= 0xfffff000;
    res |= ((str >> 7u) & 1u) << 11u;
    res |= ((str >> 25u) & 63u) << 5u;
    res |= ((str >> 8u) & 15u) << 1u;
    return res;
}

int32 instruction::get_U_imm(const int32 &str) {
    return str & 0xfffff000;
}

int32 instruction::get_J_imm(const int32 &str) {
    int32 res = 0u;
    if ((str >> 31u) == 1u)
        res |= 0xfff00000;
    res |= str & (int32)0x000ff000;
    res |= ((str >> 20u) & 1u) << 11u;
    res |= ((str >> 21u) & 1023u) << 1u;
    return res;
}

instruction::instruction() {
    name = NAME0;
    imm = funct3 = funct7 = 0u;
    rs1 = rs2 = rd = 0u;
    opcode = str = 0u;
}

instruction::instruction(int32 _str) {
    name = NAME0;
    imm = funct3 = funct7 = 0u;
    rs1 = rs2 = rd = 0u;

    str = _str;
    opcode = str & 127u;


    switch (opcode) {
        case 0x37: // LUI U
            name = LUI;
            rd = (str >> 7u) & 31u;
            imm = get_U_imm(str);
            break;
        case 0x17: // AUIPC U
            name = AUIPC;
            rd = (str >> 7u) & 31u;
            imm = str & 0xfffff000;
            break;
        case 0x6f: // JAL J
            name = JAL;
            rd = (str >> 7u) & 31u;
            imm = get_J_imm(str);
            break;
        case 0x67: // JALR I
            name = JALR;
            rd = (str >> 7u) & 31u;
            imm = get_I_imm(str);
            rs1 = (str >> 15u) & 31u;
            funct3 = 0u;
            break;
        case 0x63: // BEQ BNE BLT BGE BLTU BGEU B
            imm = get_B_imm(str);
            rs1 = (str >> 15u) & 31u;
            rs2 = (str >> 20u) & 31u;
            funct3 = (str >> 12u) & 7u;
            switch (funct3) {
                case 0: name = BEQ; break;
                case 1: name = BNE; break;
                case 4: name = BLT; break;
                case 5: name = BGE; break;
                case 6: name = BLTU; break;
                case 7: name = BGEU; break;
            }
            break;
        case 0x03: // LB LH LW LBU LHU I
            imm = get_I_imm(str);
            rs1 = (str >> 15u) & 31u;
            rd = (str >> 7u) & 31u;
            funct3 = (str >> 12u) & 7u;
            switch (funct3) {
                case 0: name = LB; break;
                case 1: name = LH; break;
                case 2: name = LW; break;
                case 4: name = LBU; break;
                case 5: name = LHU; break;
            }
            break;
        case 0x23: // SB SH SW S
            imm = get_S_imm(str);
            rs1 = (str >> 15u) & 31u;
            rs2 = (str >> 20u) & 31u;
            funct3 = (str >> 12u) & 7u;
            switch (funct3) {
                case 0: name = SB; break;
                case 1: name = SH; break;
                case 2: name = SW; break;
            }
            break;
        case 0x13: // ADDI SLTI SLTIU XORI ORI ANDI SLLI SRLI SRAI I
            imm = get_I_imm(str);
            rs1 = (str >> 15u) & 31u;
            rd = (str >> 7u) & 31u;
            funct3 = (str >> 12u) & 7u;
            switch (funct3) {
                case 0: name = ADDI; break;
                case 2: name = SLTI; break;
                case 3: name = SLTIU; break;
                case 4: name = XORI; break;
                case 6: name = ORI; break;
                case 7: name = ANDI; break;

                case 1: name = SLLI; break;
                case 5:
                    switch (str >> 30u) {
                        case 0: name = SRLI; break;
                        case 1: name = SRAI; break;
                    }
                    break;
            }
            break;
        case 0x33: // ADD SUB SLL SLT SLTU XOR ARL SRA OR AND R
            rs1 = (str >> 15u) & 31u;
            rs2 = (str >> 20u) & 31u;
            rd = (str >> 7u) & 31u;
            funct3 = (str >> 12u) & 7u;
            switch (funct3) {
                case 0:
                    switch (str >> 30u) {
                        case 0: name = ADD; break;
                        case 1: name = SUB; break;
                    }
                    break;
                case 1: name = SLL; break;
                case 2: name = SLT; break;
                case 3: name = SLTU; break;
                case 4: name = XOR; break;
                case 5:
                    switch (str >> 30u) {
                        case 0: name = SRL; break;
                        case 1: name = SRA; break;
                    }
                    break;
                case 6: name = OR; break;
                case 7: name = AND; break;
            }
            break;
    }
}

void instruction::IF() {

}

void instruction::ID() {

}

void instruction::EX() {
    switch (name) {
        case LUI: reg[rd] = imm, pc += 4; break;
        case AUIPC: reg[rd] += imm, pc += 4; break;

        case JAL: reg[rd] = pc + 4, pc += imm; break;
        case JALR: reg[rd] = pc + 4, pc = (imm + reg[rs1]) & (-2u); break;

        case BEQ: pc += reg[rs1] == reg[rs2] ? imm : 4; break;
        case BNE: pc += reg[rs1] != reg[rs2] ? imm : 4; break;
        case BLT: pc += (int)reg[rs1] < (int)reg[rs2] ? imm : 4; break;
        case BGE: pc += (int)reg[rs1] >= (int)reg[rs2] ? imm : 4; break;
        case BLTU: pc += reg[rs1] < reg[rs2] ? imm : 4; break;
        case BGEU: pc += reg[rs1] >= reg[rs2] ? imm : 4; break;

        case ADDI: reg[rd] = reg[rs1] + imm, pc += 4; break;
        case SLTI: reg[rd] = (int)reg[rs1] < (int)imm, pc += 4; break;
        case SLTIU: reg[rd] = reg[rs1] < imm, pc += 4; break;
        case XORI: reg[rd] = reg[rs1] ^ imm, pc += 4; break;
        case ORI: reg[rd] = reg[rs1] | imm, pc += 4; break;
        case ANDI: reg[rd] = reg[rs1] & imm, pc += 4; break;

        case SLLI: reg[rd] = reg[rs1] << (imm & 31u), pc += 4; break;
        case SRLI: reg[rd] = reg[rs1] >> (imm & 31u), pc += 4; break;
        case SRAI: reg[rd] = (int)(reg[rs1]) >> (imm & 31u), pc += 4; break;

        case ADD: reg[rd] = reg[rs1] + reg[rs2], pc += 4; break;
        case SUB: reg[rd] = reg[rs1] - reg[rs2], pc += 4; break;

        case SLL: reg[rd] = reg[rs1] << (reg[rs2] & 31u), pc += 4; break;
        case SRL: reg[rd] = reg[rs1] >> (reg[rs2] & 31u), pc += 4; break;
        case SRA: reg[rd] = (int)(reg[rs1]) >> (reg[rs2] & 31u), pc += 4; break;

        case SLT: reg[rd] = (int)reg[rs1] < (int)reg[rs2], pc += 4; break;
        case SLTU: reg[rd] = reg[rs1] < reg[rs2], pc += 4; break;

        case XOR: reg[rd] = reg[rs1] ^ reg[rs2], pc += 4; break;
        case OR: reg[rd] = reg[rs1] | reg[rs2], pc += 4; break;
        case AND: reg[rd] = reg[rs1] & reg[rs2], pc += 4; break;

        default: break;
    }
    reg[0] = 0;
}

void instruction::MEM() {
    switch (name) {
        case LB:
            char res_LB;
            memcpy(&res_LB, mem + (reg[rs1] + imm), sizeof(char));
            reg[rd] = (int32)res_LB;
            pc += 4;
            break;
        case LH:
            short res_LH;
            memcpy(&res_LH, mem + (reg[rs1] + imm), sizeof(short));
            reg[rd] = (int32)res_LH;
            pc += 4;
            break;
        case LW:
            memcpy(&reg[rd], mem + (reg[rs1] + imm), sizeof(int32));
            pc += 4;
            break;
        case LBU:
            uchar res_LBU;
            memcpy(&res_LBU, mem + (reg[rs1] + imm), sizeof(uchar));
            reg[rd] = (int32)res_LBU;
            pc += 4;
            break;
        case LHU:
            unsigned short res_LHU;
            memcpy(&res_LHU, mem + (reg[rs1] + imm), sizeof(unsigned short));
            reg[rd] = (int32)res_LHU;
            pc += 4;
            break;

        case SB:
            char res_SB;
            res_SB = (char)rs2;
            memcpy(mem + (reg[rs1] + imm), &reg[res_SB], sizeof(char));
            pc += 4;
            break;
        case SH:
            short res_SH;
            res_SH = (short)rs2;
            memcpy(mem + (reg[rs1] + imm), &reg[res_SH], sizeof(short));
            pc += 4;
            break;
        case SW:
            memcpy(mem + (reg[rs1] + imm), &reg[rs2], sizeof(int32));
            pc += 4;
            break;

        default:
            break;
    }
}

void instruction::WB() {

}

void instruction::show_ins() {
    printf("pc = %x\n", pc);
    std::cout << "name = " << name << std::endl;
    std::cout << "imm = " << std::bitset<32>(imm) << std::endl;
    std::cout << "funct3 = " << std::bitset<3>(funct3) << ", funct7 = " << std::bitset<3>(funct7) << std::endl;
    std::cout << "rs1 = " << std::bitset<5>(rs1) << ", rs2 = " << std::bitset<5>(rs2) << std::endl;
    std::cout << "rd = " << std::bitset<5>(rd) << std::endl;
    std::cout << "opcode = " << std::bitset<7>(opcode) << std::endl;
    printf("str = %x\n", str);
    std::cout << std::endl;
}
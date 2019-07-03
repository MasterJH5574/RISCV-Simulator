#ifndef PPCA_RISC_V_STAGES_H
#define PPCA_RISC_V_STAGES_H

#include "instructions.h"

class IF_ID;
class ID_EX;
class EX_MEM;
class MEM_WB;


class IF_ID {
public:
    bool empty;
    int32 _pc;
    int32 ins_str;

public:
    IF_ID();
    void clear();
    void push();
    void execute(ID_EX &id_ex);
};

class ID_EX {
public:
    bool empty;
    int32 _pc;
    int32 imm;
    int32 v_rs1, v_rs2;
    instruction ins;

public:
    ID_EX();
    void clear();
    void execute(EX_MEM &ex_mem);
};

class EX_MEM {
public:
    bool empty;
    int32 _pc;
    int32 imm;
    int32 v_rs1, v_rd;
    instruction ins;

public:
    EX_MEM();
    void clear();
    void execute(MEM_WB &mem_wb);
};

class MEM_WB {
public:
    bool empty;
    int32 _pc;
    int32 v_rd;
    instruction ins;

public:
    MEM_WB();
    void clear();
    void execute();
};

#endif //PPCA_RISC_V_STAGES_H

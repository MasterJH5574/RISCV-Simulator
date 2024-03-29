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
    void push();
    void execute(ID_EX *id_ex, EX_MEM *ex_mem, MEM_WB *mem_wb);

public:
    void display();
};

class ID_EX {
public:
    bool empty;
    int32 _pc;
    int32 imm;
    int32 v_rs1, v_rs2;

    ins_name name;
    int32 rd;

public:
    ID_EX();
    void execute(EX_MEM *ex_mem, IF_ID *if_id);

public:
    void display();
};

class EX_MEM {
public:
    bool empty;
    int32 imm;
    int32 v_rs1, v_rd;

    ins_name name;
    int32 rd;

    int time;

public:
    EX_MEM();
    void execute(MEM_WB *mem_wb);

public:
    void display();
};

class MEM_WB {
public:
    bool empty;
    int32 v_rd;

    ins_name name;
    int32 rd;

public:
    MEM_WB();
    void execute();

public:
    void display();
};

#endif //PPCA_RISC_V_STAGES_H

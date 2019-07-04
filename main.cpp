#include <bits/stdc++.h>
#include "instructions.h"
#include "global.h"
#include "stages.h"

using namespace std;

bool check_end(const IF_ID &if_id, const ID_EX &id_ex, const EX_MEM &ex_mem, const MEM_WB &mem_wb) {
    if (if_id.empty)
        return false;
    if (if_id.ins_str != 0x00c68223)
        return false;
    return id_ex.empty && ex_mem.empty && mem_wb.empty;
}

int main() {
    //freopen("data.in", "r", stdin);

    char buf[12] = {0};
    int32 mem_start = 0;

    while (scanf("%s", buf) != EOF) {
        if (buf[0] == '@')
            sscanf(buf + 1, "%x", &mem_start);
        else {
            sscanf(buf, "%x", &mem[mem_start]);
            scanf("%x", &mem[mem_start + 1]);
            scanf("%x", &mem[mem_start + 2]);
            scanf("%x", &mem[mem_start + 3]);
            mem_start += 4;
        }
    }

    pc = 0;
    IF_ID if_id;
    ID_EX id_ex;
    EX_MEM ex_mem;
    MEM_WB mem_wb;

    // success branch prediction: 33765839
    while (!check_end(if_id, id_ex, ex_mem, mem_wb)) {
        if (!mem_wb.empty)
            mem_wb.execute();
        if (!ex_mem.empty && mem_wb.empty)
            ex_mem.execute(&mem_wb);
        if (!id_ex.empty && ex_mem.empty)
            id_ex.execute(&ex_mem, &if_id);
        if (!if_id.empty && id_ex.empty)
            if_id.execute(&id_ex, &ex_mem, &mem_wb);
        if (if_id.empty)
            if_id.push();
    }
    //printf("%d\n", success_cnt);
    printf("%u\n", ((int32)reg[10]) & 255u);
    return 0;
}
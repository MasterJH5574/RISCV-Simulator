#include <bits/stdc++.h>
#include "instructions.h"
#include "global.h"

using namespace std;

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
    while (true) {
        int32 str;
        memcpy(&str, mem + pc, sizeof(int32));
        if (str == 0x00c68223)
            break;

        instruction ins(str);

        //printf("reg[a5] = %u\n", reg[15]);
        //ins.show_ins();

        ins.EX();
        ins.MEM();
    }
    return 0;
}
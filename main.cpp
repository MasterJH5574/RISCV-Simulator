#include <bits/stdc++.h>
#include "instructions.h"
#include "global.h"

using namespace std;



int main() {
    char buf[12];
    int32 mem_start;

    while (scanf("%s", buf) != EOF) {
        if (buf[0] == '@')
            sscanf(buf + 1, "%x", &mem_start);
        else {
            sscanf(buf, "%x", &mem[mem_start + 3]);
            scanf("%x", &mem[mem_start + 2]);
            scanf("%x", &mem[mem_start + 1]);
            scanf("%x", &mem[mem_start]);
        }
    }


    /*
    while (scanf("%s", str[3]) != EOF) {
    scanf("%s %s %s", str[2], str[1], str[0]);
    int32 ins_num = 0;
    for (int i = 0; i < 4; i++) {
        ins_num = (ins_num << 4u) | (int32)(str[i][0] <= '9' ? str[i][0] - '0' : str[i][0] - 'A' + 10);
        ins_num = (ins_num << 4u) | (int32)(str[i][1] <= '9' ? str[i][1] - '0' : str[i][1] - 'A' + 10);
    }
    cout << "ins    = " << bitset<32>(ins_num) << endl;
    instruction ins(ins_num);
    cout << "name   = " << ins.name << endl;
    cout << "imm    = " << ins.imm << endl;
    cout << "funct3 = " << bitset<3>(ins.funct3) << endl;
    cout << "funct7 = " << bitset<3>(ins.funct7) << endl;
    cout << "rs1    = " << bitset<5>(ins.rs1) << endl;
    cout << "rs2    = " << bitset<5>(ins.rs2) << endl;
    cout << "rd     = " << bitset<5>(ins.rd) << endl;
    cout << "opcode = " << bitset<7>(ins.opcode) << endl;
    cout << endl;
    }
     */
    return 0;
}
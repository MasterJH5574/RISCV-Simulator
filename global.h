#ifndef PPCA_RISC_V_GLOBAL_H
#define PPCA_RISC_V_GLOBAL_H

#include "predictor.h"

typedef unsigned int int32;
typedef unsigned char uchar;

extern uchar mem[8388608];

extern int32 reg[32], pc;

extern int used[32];

extern predictor pred[32];

//extern int success_cnt;

#endif //PPCA_RISC_V_GLOBAL_H

#include "global.h"

uchar mem[8388608];

int32 reg[32], pc;

int used[32];

predictor pred[32];

//int success_cnt;
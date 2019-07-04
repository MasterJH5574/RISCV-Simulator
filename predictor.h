#ifndef PPCA_RISC_V_PREDICTOR_H
#define PPCA_RISC_V_PREDICTOR_H


class predictor {
public:
    int history;
    int cnt[4];

public:
    predictor();
};


#endif //PPCA_RISC_V_PREDICTOR_H

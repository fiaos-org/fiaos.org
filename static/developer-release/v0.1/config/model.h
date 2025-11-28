#ifndef MODEL_H
#define MODEL_H

struct BioModelParams {
    float k_desiccation;
    float k_recovery;
    float k_damage;
    float threshold_opt;
    float threshold_crit;
};

const BioModelParams DEFAULT_ORGANISM = {
    2.5,
    0.5,
    0.1,
    15.0,
    85.0
};

#endif // MODEL_H

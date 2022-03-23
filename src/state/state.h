//
// Created by saajan on 3/19/22.
//

#ifndef NOCAP_CLION_STATE_H
#define NOCAP_CLION_STATE_H

#include <unordered_set>
#include "regex.h"

struct state{
    std::unordered_set<REG*> regVars= {};
};

#endif //NOCAP_CLION_STATE_H

//
// Created by saajan on 3/19/22.
//

#ifndef NOCAP_CLION_STATE_H
#define NOCAP_CLION_STATE_H

#include <unordered_map>
#include <string>
#include "regex.h"

struct state{
    std::unordered_map<std::string, REG*> regVars= {};

    void addVar(string name, REG *pReg) {
        if(regVars.count(name)) regVars.erase(name);
        regVars.insert({name,pReg});
    }
    REG* getVar(string name) {
        if(regVars.count(name)){
            return regVars.find(name)->second;
        }
        return nullptr;
    }
};

#endif //NOCAP_CLION_STATE_H

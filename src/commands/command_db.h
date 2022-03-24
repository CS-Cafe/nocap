//
// Created by saajan on 3/16/22.
//

#ifndef NOCAP_CLION_COMMAND_DB_H
#define NOCAP_CLION_COMMAND_DB_H

#include <map>
#include <string>
#include <vector>
#include "command.h"
#include "cmd_lexer.h"
#include "state.h"

class command_db {
  private:
    std::map<CmdTokenType,command*> * cmd_registy;
  public:
    command_db();
    void addCommand(command *cmd);
    void executeCommand(CmdToken command, std::vector<CmdToken> args, state *pstate);

    std::map<CmdTokenType, command *> *getRegistry();
};


#endif //NOCAP_CLION_COMMAND_DB_H

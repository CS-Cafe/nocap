//
// Created by saajan on 3/16/22.
//

#include "command_db.h"

command_db::command_db() {
    cmd_registy=new map<CmdTokenType,command*>();
}

void command_db::addCommand(command *cmd) {
    cmd_registy->insert({cmd->enumName,cmd});
}

void command_db::executeCommand(CmdToken command, std::vector<CmdToken> args, state *pstate) {
    if(command.token==ARGS){
        cout << "Invalid command. Use `help` for a list of commands" <<endl;
        return;
    }
    cmd_registy->find(command.token)->second->execute(args,pstate);
}

std::map<CmdTokenType, command *> *command_db::getRegistry() {
    return cmd_registy;
}



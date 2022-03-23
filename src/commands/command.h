//
// Created by saajan on 3/16/22.
//

#ifndef NOCAP_CLION_COMMAND_H
#define NOCAP_CLION_COMMAND_H

#include "string"
#include "functional"
#include "cmd_lexer.h"
#include "unordered_set"
#include "map"
#include "vector"
#include "iostream"
#include "terminal.h"
/*
#include "reg_lexer.h"
*/

using std::cout, std::endl;

struct command{
    CmdTokenType enumName;
    std::string fullName;
    std::string manual;
    std::function<void(std::vector<CmdToken>)> execute;
    std::unordered_set<command*> subCommands;
    std::map<CmdTokenType,command*> * registryRef;
    command(std::map<CmdTokenType,command*> * regRef):enumName(ERROR),execute([](std::vector<CmdToken> args){}),subCommands({}),registryRef(regRef){};
    void addSubCommand(command* cmd){
        subCommands.insert(cmd);
    }
};

struct helpCMD:command{
    helpCMD(std::map<CmdTokenType,command*> * regRef):command(regRef){
        enumName = HELP;
        fullName = "help";
        manual = "Seriously? If you dont know how to use `help` then how are you here?";
        execute = [regRef](std::vector<CmdToken> args){
            if(!args.empty()){
                if(!regRef->count(args.at(0).token)){
                    cout << "Invalid command. Use `help` for a list of commands" <<endl;
                    return;
                }
                cout << regRef->find(args.at(0).token)->second->manual <<endl;
                return;
            }
            cout << "NoCAP: The Novel Compiler Automation Program\n";
            cout << "These commands are internally defined.\n";
            cout << "\tType 'help' to see this list.\n";
            cout << "\tType 'help <name>' to learn about a specific command.\n\n";
            for(auto cmd: *regRef){
                cout << cmd.second->fullName << "\n";
            }
            cout << endl;
        };
    };
};

struct quitCMD:command{
    quitCMD():command(nullptr){
        enumName = QUIT;
        fullName = "quit";
        manual = "Quits the app";
        execute = [](std::vector<CmdToken> args){
            if(!args.empty()){
                cout << "Invalid subcommand. Use `help` for a list of commands" <<endl;
                return;
            }
            exit(0);
        };
    };
};

struct clrCMD:command{
    clrCMD():command(nullptr){
        enumName = CLEAR;
        fullName = "clear";
        manual = "Clears the screen";
        execute = [](std::vector<CmdToken> args){
            if(!args.empty()){
                cout << "Invalid subcommand. Use `help` for a list of commands" <<endl;
                return;
            }
            cout << CLEAR_TERM;
        };
    };
};

#endif //NOCAP_CLION_COMMAND_H

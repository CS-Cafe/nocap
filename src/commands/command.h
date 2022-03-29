//
// Created by saajan on 3/16/22.
//

#ifndef NOCAP_CLION_COMMAND_H
#define NOCAP_CLION_COMMAND_H

#include "string"
#include "functional"
#include "cmd_lexer.h"
#include "parser.h"
#include "unordered_set"
#include "map"
#include "vector"
#include "iostream"
#include "terminal.h"
#include "state.h"
/*
#include "reg_lexer.h"
*/

using std::cout, std::endl;

struct command{
    CmdTokenType enumName;
    std::string fullName;
    bool indexable = true;
    std::string manual;
    std::function<void(std::vector<CmdToken>,state *pstate)> execute;
    std::unordered_set<command*> subCommands;
    std::map<CmdTokenType,command*> * registryRef;
    command(std::map<CmdTokenType,command*> * regRef):enumName(ERROR),execute([](std::vector<CmdToken> args,state *pstate){}),subCommands({}),registryRef(regRef){};
    void addSubCommand(command* cmd){
        subCommands.insert(cmd);
    }
};

struct helpCMD:command{
    helpCMD(std::map<CmdTokenType,command*> * regRef):command(regRef){
        enumName = HELP;
        fullName = "help";
        manual = "Seriously? If you dont know how to use `help` then how are you here?";
        execute = [regRef](std::vector<CmdToken> args, state* pstate){
            if(!args.empty()){
                int max = 0;
                std::string manOut;
                for(auto arg : args){
                    if(!regRef->count(arg.token)){
                        cout << "Invalid command. Use `help` for a list of commands" <<endl;
                        return;
                    }
                    command* curr = regRef->find(arg.token)->second;
                    if(curr->fullName.size() > max){
                        max = curr->fullName.size();
                        manOut = curr->manual;
                    }
                }
                if(!regRef->count(args.at(0).token)){
                    cout << "Invalid command. Use `help` for a list of commands" <<endl;
                    return;
                }
                cout << manOut <<endl;
                return;
            }
            cout << "NoCAP: The Novel Compiler Automation Program\n";
            cout << "These commands are internally defined.\n";
            cout << "\tType 'help' to see this list.\n";
            cout << "\tType 'help <name>' to learn about a specific command.\n\n";
            for(auto cmd: *regRef){
                if(!cmd.second->indexable) continue;
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
        execute = [](std::vector<CmdToken> args, state* pstate){
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
        execute = [](std::vector<CmdToken> args, state* pstate){
            if(!args.empty()){
                cout << "Invalid subcommand. Use `help` for a list of commands" <<endl;
                return;
            }
            cout << CLEAR_TERM;
        };
    };
};

struct mkCMD:command{
    mkCMD(std::map<CmdTokenType,command*> * regRef):command(regRef){
        enumName = MK;
        fullName = "mk";
        manual = "Make something! Be it a rule, grammar, or regex, this is your one stop shop.\n\nUsage:\n\nmk <grammar | regex | rule | terminal>";
        execute = [=](std::vector<CmdToken> args, state* pstate){
            if(args.empty()){
                cout << "Invalid subcommand. Use `help` for a list of commands" <<endl;
                return;
            }

            CmdToken subcmd = args.at(0);
            switch (subcmd.token) {
                case REGEX:
                    regRef->find(subcmd.token)->second->execute(std::vector(args.begin()+1,args.end()),pstate);
                    break;
                default:
                    cout << "Invalid subcommand. Use `help` for a list of commands" <<endl;
                    break;
            }
        };
    };
};

struct mkRegexCMD:command{
    mkRegexCMD(std::map<CmdTokenType,command*> * regRef):command(regRef){
        enumName = REGEX;
        fullName = "mk regex";
        indexable = false;
        manual = "Make a regular expression.\n\nUsage:\n\n mk regex <regular-expression>\n\nWhere <regular-expression> is a valid regular expression abiding by the following grammar (| is a literal pipe character):\n\n\tR -> CHAR\n\tR -> <name of a previously defined regex>\n\tR -> UNDERSCORE\n\tR -> (R).(R)\n\tR -> (R)|(R)";
        execute = [=](std::vector<CmdToken> args, state* pstate){
            if(args.size()!=2 || args.at(1).token!=ARGS){
                cout << "Invalid subcommand. Use `help` for a list of commands" <<endl;
                return;
            }
            if(args.at(0).lexeme.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")!=string::npos){
                cout << "Invalid variable name. Please try again and choose another name for this regex." <<endl;
            }
            try {
                reg::Parser tempParser;
                tempParser.setString(args.at(1).lexeme);
                REG *output = new REG(tempParser.parse_input(pstate));
                pstate->addVar(args.at(0).lexeme, output);
            }catch(reg::regex_invalid_err & e){
                cout << e.what() <<endl;
            }
        };
    };
};

#endif //NOCAP_CLION_COMMAND_H

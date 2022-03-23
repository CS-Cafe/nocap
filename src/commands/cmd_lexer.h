//
// Created by saajan on 3/16/22.
//

#ifndef NOCAP_CLION_CMD_LEXER_H
#define NOCAP_CLION_CMD_LEXER_H

#include "enum_mapper.h"
#include "vector"
#include "unordered_map"
#include "string"

using  std::vector, std::string, std::begin,std::end;

MAPPED_ENUM(CmdTokenType, ERROR, ARGS, CLEAR, HELP, QUIT, MK, GRAMMAR, RULE, REGEX, TERMINAL, DISP, RM, DEFAULT)

struct CmdToken{
    CmdTokenType token;
    string lexeme;
    CmdToken():token(ERROR){};
};

class cmd_lexer {
  public:
    static vector<CmdToken> lexInput(string in);
};


#endif //NOCAP_CLION_CMD_LEXER_H

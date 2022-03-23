/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 *
 * Modified by Jacob Anderson, 2022
 * Last Modified: 02/18/2022
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include "reg_lexer.h"
#include "regex.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
namespace reg {
    class Parser {
      public:
        void ConsumeAllInput();

        REG parse_input();

        void lexInput(std::string in);

      private:
        reg_lexer lexer;

        void syntax_error();

        reg_lexer::reg_tok expect(REG_RESERVED expected_type);

        REG parse_in();

        REG parse_expr();

        REG parse_expr_modifier(REG regToMod);



        bool verifyLackOfEpDerivations(REG reg);
    };
};
#endif

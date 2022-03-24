/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 * Modified by Jacob Anderson, 2022
 * Last Modified: 02/18/2022
 */
#include "parser.h"
#include <cstdlib>
#include <iostream>

using namespace std;
namespace reg {
    void Parser::syntax_error() {
        cout << "SYNTAX ERROR\n";
        exit(1);
    }

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
    reg_lexer::reg_tok Parser::expect(REG_RESERVED expected_type) {
        reg_lexer::reg_tok t = lexer.get_token();
        if (t.token != expected_type)
            syntax_error();
        return t;
    }

// Parsing

    REG Parser::parse_input(state* pstate) {
        REG res = parse_expr(pstate);
        expect(END_OF_FILE);
        if (!verifyLackOfEpDerivations(res)) return REG();
        return res;
    }

    REG Parser::parse_expr(state* pstate) {
        reg_lexer::reg_tok t = lexer.peek(1);
        // cout << "PARSE_EXPR: "; t.Print();
        if (t.token == CHAR) {
            reg_lexer::reg_tok charToken = expect(CHAR);
            return REG_Factory::createReg(charToken.lexeme[0]);
        }
        if (t.token == VARIABLE) {
            reg_lexer::reg_tok varToken = expect(VARIABLE);
            REG toCopy = pstate->getVar(varToken.lexeme);
            return REG_Factory::copy(toCopy);
        } else if (t.token == LPAREN) {
            expect(LPAREN);
            REG parsedREG = parse_expr(pstate);
            expect(RPAREN);
            return parse_expr_modifier(parsedREG,pstate);
        } else if (t.token == UNDERSCORE) {
            expect(UNDERSCORE);
            return REG_Factory::createReg('_');
        } else {
            syntax_error();
        }
        // cout << "BASSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS" << endl;
        return {false};
    }

    REG Parser::parse_expr_modifier(REG regToMod,state * pstate) {
        reg_lexer::reg_tok t = lexer.peek(1);
        // cout << "PARSE_MOD: "; t.Print();
        if (t.token == DOT) {
            expect(DOT);
            expect(LPAREN);
            REG rhs = parse_expr(pstate);
            expect(RPAREN);
            return REG_Factory::createReg(regToMod, CONCAT_OP, rhs);
        } else if (t.token == OR) {
            expect(OR);
            expect(LPAREN);
            REG rhs = parse_expr(pstate);
            expect(RPAREN);
            return REG_Factory::createReg(regToMod, OR_OP, rhs);
        } else if (t.token == STAR) {
            expect(STAR);
            return REG_Factory::createReg(regToMod, STAR_OP);
        } else if (t.token == LBRACK) {
            // EC
            expect(LBRACK);
            int low = stoi(expect(NUM).lexeme);
            expect(COMMA);
            int hi = stoi(expect(NUM).lexeme);
            if (low > hi) syntax_error();
            REG res;
            for (int i = low; i <= hi; i++) {
                if (i == low) {
                    res = REG_Factory::finiteConcat(regToMod, i);
                    continue;
                }
                REG temp = REG_Factory::finiteConcat(regToMod, i);
                res = REG_Factory::createReg(res, OR_OP, temp);
            }
            expect(RBRACK);
            return res;
        } else {
            syntax_error();
        }
        // cout << "GOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOD" << endl;
        return {false};
    }

    /*void Parser::lexInput(std::string in) {
        size_t currPos = 0;
        if (in.size() <= 0) return;
        do {
            string maxToken = "";
            int maxTokenLength = 0;
            for (std::vector<std::string>::iterator ii = orderedTokens.begin(); ii != orderedTokens.end(); ii++) {
                std::unordered_map<std::string, REG>::iterator i = tokenREGs.find(*ii);
                int currREGSubLength = REG_Util::processInput(i->second, in, currPos);
                if (currREGSubLength > maxTokenLength) {
                    maxTokenLength = currREGSubLength;
                    maxToken = i->first;
                }
            }
            if (maxTokenLength <= 0) {
                cout << "ERROR" << endl;
                exit(1);
            }
            try {
                cout << maxToken << " , \"" << in.substr(currPos, maxTokenLength) << "\"" << endl;
            }
            catch (out_of_range &e) {
                cout << maxToken << " , \"" << in.substr(currPos) << "\"" << endl;
            }
            currPos += maxTokenLength;
            currPos = in.find_first_not_of(' ', currPos);
        } while (currPos < in.size());
    }*/

    bool Parser::verifyLackOfEpDerivations(REG toCheck) {
        if (toCheck.accept == toCheck.start) return false;
        if (REG_Util::calculateEpClosure({toCheck.start}).count(toCheck.accept)) {
            return false;
        }
        return true;
    }

    void Parser::setString(string in) {
        lexer.process(in);
    }
}
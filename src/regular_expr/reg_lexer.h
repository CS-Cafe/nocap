//
// Created by saajan on 3/17/22.
//

#ifndef NOCAP_CLION_REG_LEXER_H
#define NOCAP_CLION_REG_LEXER_H

#include <vector>
#include <string>

using std::vector, std::string;
namespace reg {

    enum REG_RESERVED {
        LPAREN,
        RPAREN,
        LBRACK,
        RBRACK,
        DOLLAR,
        BACKSLASH,
        COMMA,
        DOT,
        STAR,
        OR,
        UNDERSCORE,
        DIGIT,
        CHAR,
        NUM,
        VARIABLE,
        ERROR,
        END_OF_FILE
    };

    class reg_lexer {
      private:
        int pos = -1;

      private:
        string REG_RESERVED_MAP = "(){}$\\,.*|_";
        struct preproc_tok {
            REG_RESERVED token = ERROR;
            char lexeme = '\0';
        };
      public:
        struct reg_tok {
            REG_RESERVED token = ERROR;
            string lexeme;
        };
      private:
        vector<reg_tok> buf = {};

        vector<preproc_tok> preprocess(string in) {
            vector<preproc_tok> res = {};
            bool forceCharMode = false;
            for (char i: in) {
                preproc_tok temp;
                if (forceCharMode) {
                    forceCharMode = false;
                    temp.token = CHAR;
                    temp.lexeme = i;
                    res.push_back(temp);
                    continue;
                }
                int reservedIndex = REG_RESERVED_MAP.find(i);
                if (reservedIndex >= 0) {
                    temp.token = static_cast<REG_RESERVED>(reservedIndex);
                    if (temp.token == BACKSLASH) {
                        forceCharMode = true;
                        continue;
                    } else {
                        temp.lexeme = i;
                        res.push_back(temp);
                    }
                } else {
                    temp.token = std::isdigit(i) ? DIGIT : CHAR;
                    temp.lexeme = i;
                    res.push_back(temp);
                }
            }
        };

        void process(string unproc) {
            vector<preproc_tok> in = preprocess(unproc);
            vector<reg_tok> res = {};
            enum Mode {
                STANDARD,
                NUM_MODE,
                VAR_MODE
            } mode = STANDARD;
            reg_tok temp;
            string tString;
            for (int i = 0; i < in.size(); i++) {
                if (mode == STANDARD) {
                    if (in[i].token == LBRACK) {
                        temp.token = in[i].token;
                        temp.lexeme = in[i].lexeme;
                        res.push_back(temp);
                        mode = NUM_MODE;
                    } else if (in[i].token == DOLLAR) {
                        mode = VAR_MODE;
                    } else {
                        temp.token = in[i].token;
                        temp.lexeme = in[i].lexeme;
                        res.push_back(temp);
                    }
                } else if (mode == NUM_MODE) {
                    if (in[i].token == DIGIT) {
                        tString += in[i].lexeme;
                    } else if (in[i].token == COMMA) {
                        temp.token = NUM;
                        temp.lexeme = tString;
                        tString = "";
                        res.push_back(temp);
                    } else {
                        temp.token = NUM;
                        temp.lexeme = tString;
                        tString = "";
                        res.push_back(temp);
                        mode = STANDARD;
                    }
                } else {
                    if (in[i].token == CHAR || in[i].token == DIGIT) {
                        tString += in[i].lexeme;
                    } else {
                        temp.token = VARIABLE;
                        temp.lexeme = tString;
                        tString = "";
                        res.push_back(temp);
                        mode = STANDARD;
                        i--;
                    }
                }
            }
            buf = res;
        };
      public:
        reg_tok peek(int i) {
            if (i <= 0) return {ERROR, ""};
            if (pos + i >= buf.size()) return {END_OF_FILE, ""};
            return buf[pos + i];
        };

        reg_tok get_token() {
            reg_tok res = peek(1);
            pos++;
            return res;
        };
    };
};
#endif //NOCAP_CLION_REG_LEXER_H

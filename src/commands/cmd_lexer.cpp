//
// Created by saajan on 3/16/22.
//

#include "cmd_lexer.h"

vector<CmdToken> cmd_lexer::lexInput(string in) {
    vector<CmdToken> res = {};
    auto tokMap = unordered_map<string, CmdTokenType>(begin(CmdTokenType_MAP.smap),
                                                      end(CmdTokenType_MAP.smap));
    int pos = 0;
    bool inQuote = false;

    while (pos < in.size()) {
        pos = in.find_first_not_of(' ', pos);
        char curr = in.at(pos);
        inQuote = (curr == '\"');
        if (!inQuote) {
            int newPos = in.find_first_of(' ', pos);
            if(newPos==-1) newPos = in.size();
            CmdToken temp;
            string sub = in.substr(pos, newPos - pos);
            for (auto & c: sub) c = toupper(c);
            temp.token = tokMap.count(sub) ? tokMap.find(sub)->second
                                                                    : ARGS;
            temp.lexeme = in.substr(pos, newPos - pos);
            pos = newPos;
            res.push_back(temp);
        } else {
            int newPos = in.find_first_of('\"', pos + 1);
            if(newPos==-1) newPos = in.size();
            CmdToken temp;
            string sub = in.substr(pos + 1, newPos - pos - 1);
            for (auto & c: sub) c = toupper(c);
            temp.token = tokMap.count(sub) ? tokMap.find(sub)->second : ARGS;
            temp.lexeme = in.substr(pos + 1, newPos - pos - 1);
            pos = newPos + 1;
            inQuote = false;
            res.push_back(temp);
        }
    }

    return res;
}
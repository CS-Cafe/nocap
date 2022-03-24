#include <iostream>
#include "terminal.h"
#include "command_db.h"
#include "cmd_lexer.h"
#include "state.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
using std::cout, std::cin, std::endl, std::string;

int main() {
  command_db commands;
  commands.addCommand(new helpCMD(commands.getRegistry()));
  commands.addCommand(new mkCMD(commands.getRegistry()));
  commands.addCommand(new mkRegexCMD(commands.getRegistry()));
  commands.addCommand(new quitCMD());
  commands.addCommand(new clrCMD());

  state * programState = new state();

  cout << CLEAR_TERM;
  cout << "Welcome to NoCAP: the Novel Compiler Automation Program!" << endl;
  cout << "\n\tType 'help' for help\n" << endl;

  do {
    cout << GREEN << "nocap:" << BLUE << "~" << RESET << " $ ";
    string userInput;
    getline(cin,userInput);
    auto tokens = cmd_lexer::lexInput(userInput);
    if(tokens.empty()) continue;
      commands.executeCommand(tokens.at(0), vector<CmdToken>(tokens.begin() + 1, tokens.end()), programState);
  } while (true);
}

#pragma clang diagnostic pop
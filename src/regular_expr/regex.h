#ifndef __REGEX_H__
#define __REGEX_H__

#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>

enum reg_ops
{
  OR_OP,
  CONCAT_OP,
  STAR_OP
};
struct REG_node
{
  REG_node() : first_neighbor(nullptr), first_label('\0'), second_neighbor(nullptr), second_label('\0') {}
  REG_node *first_neighbor;
  char first_label;
  REG_node *second_neighbor;
  char second_label;
  ~REG_node()
  {
    // first_label = '\0';
    // if (first_neighbor && first_neighbor->first_label)
    // {
    //   delete first_neighbor;
    //   first_neighbor = nullptr;
    // }
    // second_label = '\0';
    // if (second_neighbor && second_neighbor->second_label)
    // {
    //   delete second_neighbor;
    //   second_neighbor = nullptr;
    // }
  }
};
struct REG
{
  bool isValid = true;
  REG_node *start = nullptr;
  REG_node *accept = nullptr;
  REG(bool v) : isValid(v){};
  REG(){};
};
class REG_Factory
{
  private:
  static REG orREG(REG &reg1, REG &reg2);
  static REG concatREG(REG &reg1, REG &reg2);
  static REG starREG(REG &reg1);

  public:
  static REG createReg(char in);
  static REG createReg(REG &reg1, reg_ops op, REG &reg2);
  static REG createReg(REG &reg1, reg_ops op);
  static REG finiteConcat(REG &reg, int reps);
  static REG copy(REG &reg);
  static REG_node *dfsCopy(REG_node* follow, std::unordered_map<REG_node *,REG_node *>& visited, REG& prod, REG_node* fin);
};
class REG_Util
{
  public:
  static std::unordered_set<REG_node *> calculateEpClosure(std::unordered_set<REG_node *> in);
  static int processInput(REG &reg, std::string in, int pos);
};

#endif
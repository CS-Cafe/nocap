#include "regex.h"

REG REG_Factory::orREG(REG &reg1, REG &reg2)
{
  REG res = REG();

  REG_node *newStart = new REG_node();
  newStart->first_label = '_';
  newStart->second_label = '_';
  newStart->first_neighbor = reg1.start;
  newStart->second_neighbor = reg2.start;

  REG_node *newEnd = new REG_node();
  reg1.accept->first_label = '_';
  reg2.accept->first_label = '_';
  reg1.accept->first_neighbor = newEnd;
  reg2.accept->first_neighbor = newEnd;

  res.start = newStart;
  res.accept = newEnd;

  return res;
}

REG REG_Factory::concatREG(REG &reg1, REG &reg2)
{
  REG res = REG();
  res.start = reg1.start;
  reg1.accept->first_neighbor = reg2.start;
  reg1.accept->first_label = '_';
  res.accept = reg2.accept;
  return res;
}

REG REG_Factory::starREG(REG &reg1)
{
  REG res = REG();

  REG_node *newEnd = new REG_node();
  reg1.accept->first_label = '_';
  reg1.accept->second_label = '_';
  reg1.accept->first_neighbor = newEnd;
  reg1.accept->second_neighbor = reg1.start;

  REG_node *newStart = new REG_node();
  newStart->first_label = '_';
  newStart->second_label = '_';
  newStart->first_neighbor = reg1.start;
  newStart->second_neighbor = newEnd;

  res.start = newStart;
  res.accept = newEnd;

  return res;
}

REG REG_Factory::createReg(char in)
{
  REG res = REG();
  REG_node *startPtr = new REG_node();
  startPtr->first_label = in;
  REG_node *endPtr = new REG_node();
  startPtr->first_neighbor = endPtr;
  res.start = startPtr;
  res.accept = endPtr;
  return res;
}

REG REG_Factory::createReg(REG &reg1, reg_ops op, REG &reg2)
{
  // if(!(reg1.isValid() && reg2.isValid())) return {};
  switch (op)
  {
  case OR_OP:
    return orREG(reg1, reg2);
  case CONCAT_OP:
    return concatREG(reg1, reg2);
  default:
    throw std::runtime_error("No valid method to modify REG using given operator! ABORTING");
  }
}

REG REG_Factory::createReg(REG &reg1, reg_ops op)
{
  // if(!(reg1.isValid())) return {};
  switch (op)
  {
  case STAR_OP:
    return starREG(reg1);
  default:
    throw std::runtime_error("No valid method to modify REG using given operator! ABORTING");
  }
}

REG REG_Factory::finiteConcat(REG &reg, int reps)
{
  if (reps == 0) return createReg('_');
  REG res;
  for (int i = 1; i <= reps; i++)
  {
    if (i == 1)
    {
      res = copy(reg);
      continue;
    }
    REG temp = copy(reg);
    res = createReg(res, CONCAT_OP, temp);
  }
  return res;
}

REG REG_Factory::copy(REG &reg)
{
  
  std::unordered_map<REG_node *,REG_node *> visited = {};

  REG res;
  REG_node* newStart = dfsCopy(reg.start,visited,res,reg.accept);
  res.start = newStart;
  return res;
}

REG_node* REG_Factory::dfsCopy(REG_node* follow, std::unordered_map<REG_node *,REG_node *>& visited, REG& prod, REG_node* fin){
  if(follow == nullptr){
    return nullptr;
  }
  
  REG_node* newNode = new REG_node();
  if(follow == fin){
    prod.accept = newNode;
  }
  newNode->first_label = follow->first_label;
  newNode->second_label = follow->second_label;
  visited.insert({follow,newNode});
  if(visited.count(follow->first_neighbor)){
    newNode->first_neighbor = visited.find(follow->first_neighbor)->second;
  }else{
    newNode->first_neighbor = dfsCopy(follow->first_neighbor,visited,prod,fin);
  }
  if(visited.count(follow->second_neighbor)){
    newNode->second_neighbor = visited.find(follow->second_neighbor)->second;
  }else{
    newNode->second_neighbor = dfsCopy(follow->second_neighbor,visited,prod,fin);
  }
  return newNode;
}

std::unordered_set<REG_node *> REG_Util::calculateEpClosure(std::unordered_set<REG_node *> in)
{
  std::unordered_set<REG_node *> res = {};
  res.insert(in.begin(), in.end());
  bool changed = true;
  while (changed)
  {
    changed = false;
    std::unordered_set<REG_node *> intermediate = {};
    for (std::unordered_set<REG_node *>::iterator i = res.begin(); i != res.end(); i++)
    {
      if ((*i) && (*i)->first_label == '_' && (*i)->first_neighbor) intermediate.insert((*i)->first_neighbor);
      if ((*i) && (*i)->second_label == '_' && (*i)->second_neighbor) intermediate.insert((*i)->second_neighbor);
    }
    size_t sizeB = res.size();
    res.insert(intermediate.begin(), intermediate.end());
    if (sizeB != res.size()) changed = true;
  }
  return res;
}

// Negative return values indicate a lack of match
int REG_Util::processInput(REG &reg, std::string in, int pos)
{
  std::unordered_set<REG_node *> currStates = calculateEpClosure({reg.start});
  int lastAccept = -1;
  for (size_t i = pos; i < in.size(); i++)
  {
    char currInput = in[i];
    if (currInput == ' ')
    {
      break;
    }
    std::unordered_set<REG_node *> inter = {};
    for (std::unordered_set<REG_node *>::iterator j = currStates.begin(); j != currStates.end(); j++)
    {
      if ((*j)->first_label == currInput && (*j)->first_neighbor) inter.insert((*j)->first_neighbor);
      if ((*j)->second_label == currInput && (*j)->second_neighbor) inter.insert((*j)->second_neighbor);
    }
    inter = calculateEpClosure(inter);
    if (inter.count(reg.accept)) lastAccept = i;
    currStates = inter;
  }
  return lastAccept - pos + 1;
}
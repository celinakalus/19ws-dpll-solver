#ifndef _DPLL_H_
#define _DPLL_H_

#include <vector>

#include "trail.h"

class DPLL {
private:
  int is_clause_satisfied(int clause);
  int is_clause_unsatisfied(int clause);
  int is_clause_unit(int clause, int *j);
  
  int trail_push(int x, int v, int b);
  int trail_pop(int &x, int &v, int &b);

  int backtrack();
  int decide();
  int BCP();

  std::vector<std::vector<int>> *clauses;

  int num_vars;

  std::vector<int> clause_vars_sat;
  std::vector<int> clause_vars_unsat;
  std::vector<int> clause_vars_unassigned;

  Trail *trail;

public:
  DPLL(std::vector<std::vector<int>> *clauses);
  std::vector<int> *solve();
};

#endif

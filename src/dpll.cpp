#include <vector>
#include <iostream>

#include "dpll.h"
#include "trail.h"
#include "config.h"

#define ABS(x) (x < 0 ? -x : x)
#define VAR_INDEX(x) (ABS(x) - 1)
#define NEGATIVE(x) (x < 0 ? 1 : 0) 

int is_clause_satisfied(std::vector<int> clause, Trail trail) {
  for (int i = 0; i < clause.size(); i++) {
    int x = VAR_INDEX(clause[i]);
    int neg = NEGATIVE(clause[i]);
    int v = trail.variable_value(x);

    if (v ^ neg) { return 1; }
  }

  return 0;
}

int is_formula_satisfied(std::vector<std::vector<int>> clauses, Trail trail) {
  for (int i = 0; i < clauses.size(); i++) {
    if (!is_clause_satisfied(clauses[i], trail)) { return 0; }
  }

  return 1;
}

int solve_dpll(std::vector<std::vector<int>> clauses, int *_num_vars, std::vector<int> *assignment) {
  int num_vars = -1;

  // count the number of variables
  for (int i = 0; i < clauses.size(); i++) {
    for (int j = 0; j < clauses[i].size(); j++) {
      int x = VAR_INDEX(clauses[i][j]);
      if (x + 1 > num_vars) {
        num_vars = x + 1;
      }
    }
  }
  
  *_num_vars = num_vars;

#ifdef VERBOSE
  std::cout << "#clauses: " << clauses.size() << std::endl;
  std::cout << "#vars:    " << num_vars << std::endl;
#endif

  Trail trail (num_vars);

  for (int test = 0; test < (1 << num_vars); test++) {
    int pattern = test;
    for (int x = 0; x < num_vars; x++) {
      trail.push(x, pattern & 1, 0);
      pattern = pattern >> 1;
    }

    if (is_formula_satisfied(clauses, trail)) { return 1; }

    for (int x = 0; x < num_vars; x++) {
      int a, b, c;
      trail.pop(a, b, c);
    }
  }

  return 0;
}

#include <vector>
#include <iostream>

#include "dpll.h"
#include "trail.h"
#include "config.h"

#define ABS(x) (x < 0 ? -x : x)
#define VAR_INDEX(x) (ABS(x) - 1)
#define NEGATIVE(x) (x < 0 ? 1 : 0) 

int is_clause_unsatisfied(std::vector<int> clause, Trail trail) {
  for (int i = 0; i < clause.size(); i++) {
    int x = VAR_INDEX(clause[i]);
    int neg = NEGATIVE(clause[i]);
    int v = trail.variable_value(x);

    if (!trail.variable_assigned(x)) {
      return 0;
    } else if (v ^ neg) { 
      return 0; 
    }
  }

  return 1;
}

int is_clause_satisfied(std::vector<int> clause, Trail trail) {
  for (int i = 0; i < clause.size(); i++) {
    int x = VAR_INDEX(clause[i]);
    int neg = NEGATIVE(clause[i]);
    int v = trail.variable_value(x);

    if (!trail.variable_assigned(x)) {
      continue;
    } else if (v ^ neg) { 
      return 1; 
    }
  }

  return 0;
}

int is_formula_satisfied(std::vector<std::vector<int>> clauses, Trail trail) {
  for (int i = 0; i < clauses.size(); i++) {
    if (!is_clause_satisfied(clauses[i], trail)) { return 0; }
  }

  return 1;
}

int backtrack(Trail *trail) {
  while (!trail->empty()) {
    int x, v, b;
    trail->pop(x, v, b);

    if (!b) {
      trail->push(x, !v, 1);
      return 1;
    }
  }

  return 0;
}

int decide(int num_vars, Trail *trail) {
  for (int x = 0; x < num_vars; x++) {
    if (!trail->variable_assigned(x)) {
      trail->push(x, 0, 0);
      return 1;
    }
  }

  return 0;
}

int BCP(std::vector<std::vector<int>> clauses, Trail *trail) {
  // todo: look for unit clauses, extend trail accordingly

  for (int i = 0; i < clauses.size(); i++) {
    if (is_clause_unsatisfied(clauses[i], *trail)) { return 0; }
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

  if (!BCP(clauses, &trail)) { 
    std::cout << "UNSAT in first BCP" << std::endl;
    return 0; 
  }

  while (true) {
    if (!decide(num_vars, &trail)) { 
      std::cout << "SAT in iterative decide" << std::endl;
      return 1; 
    }

    while (!BCP(clauses, &trail)) {
      if (!backtrack(&trail)) { 
        std::cout << "UNSAT in iterative BCP" << std::endl;
        return 0; 
      }
    }
  }

  return 0;
}

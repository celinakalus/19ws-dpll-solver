#include <vector>
#include <iostream>

#include "dpll.h"
#include "trail.h"
#include "config.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define VAR_INDEX(x) (ABS((x)) - 1)
#define NEGATIVE(x) ((x) < 0 ? 1 : 0) 

int DPLL::is_clause_unsatisfied(int c) {
  for (int i = 0; i < (*clauses)[c].size(); i++) {
    int x = VAR_INDEX((*clauses)[c][i]);
    int neg = NEGATIVE((*clauses)[c][i]);
    int v = trail->variable_value(x);

    if (!trail->variable_assigned(x)) {
      return 0;
    } else if (v ^ neg) { 
      return 0; 
    }
  }

  return 1;
}

int DPLL::is_clause_satisfied(int c) {
  for (int i = 0; i < (*clauses)[c].size(); i++) {
    int x = VAR_INDEX((*clauses)[c][i]);
    int neg = NEGATIVE((*clauses)[c][i]);
    int v = trail->variable_value(x);

    if (!trail->variable_assigned(x)) {
      continue;
    } else if (v ^ neg) { 
      return 1; 
    }
  }

  return 0;
}

int DPLL::is_clause_unit(int c, int *index) {
  int unassigned_count = 0;

  for (int i = 0; i < (*clauses)[c].size(); i++) {
    int x = VAR_INDEX((*clauses)[c][i]);
    int neg = NEGATIVE((*clauses)[c][i]);
    int v = trail->variable_value(x);

    if (!trail->variable_assigned(x) && unassigned_count < 1) {
      unassigned_count++;
      *index = i;
    } else if (!trail->variable_assigned(x)) {
      return 0;
    } else if (v ^ neg) { 
      return 0; 
    }
  }

  return 1;
}

int DPLL::trail_push(int x, int v, int b) {
  trail->push(x, v, b);

  return 0;
}

int DPLL::trail_pop(int &x, int &v, int &b) {
  trail->pop(x, v, b);

  return 0;
}

int DPLL::backtrack() {
  while (!trail->empty()) {
    int x, v, b;
    trail_pop(x, v, b);

    if (!b) {
      trail_push(x, !v, 1);
      return 1;
    }
  }

  return 0;
}

int DPLL::decide() {
  for (int x = 0; x < num_vars; x++) {
    if (!trail->variable_assigned(x)) {
      trail_push(x, 0, 0);
      return 1;
    }
  }

  return 0;
}

int DPLL::BCP() {
  // todo: look for unit clauses, extend trail accordingly
  while (true) {
    int unit_count = 0;

    for (int c = 0; c < clauses->size(); c++) {
      if (is_clause_unsatisfied(c)) { return 0; }
      
      int i;
      if (is_clause_unit(c, &i)) {
        int x = VAR_INDEX((*clauses)[c][i]);
        int neg = NEGATIVE((*clauses)[c][i]);

        if (neg) {
          trail_push(x, 0, 1);
        } else {
          trail_push(x, 1, 1);
        }
      }
    }

    if (unit_count == 0) { break; }
  }

  return 1;
}

DPLL::DPLL(std::vector<std::vector<int>> *_clauses) {
  clauses = _clauses;
  
  num_vars = -1;

  // count the number of variables
  for (int i = 0; i < clauses->size(); i++) {
    for (int j = 0; j < (*clauses)[i].size(); j++) {
      int x = VAR_INDEX((*clauses)[i][j]);
      if (x + 1 > num_vars) {
        num_vars = x + 1;
      }
    }
  }

#ifdef VERBOSE
  std::cout << "#clauses: " << clauses->size() << std::endl;
  std::cout << "#vars:    " << num_vars << std::endl;
#endif

  trail = new Trail(num_vars);
}

std::vector<int> *DPLL::solve() {
  if (!BCP()) { 
    std::cout << "UNSAT in first BCP" << std::endl;
    return NULL; 
  }

  while (true) {
    if (!decide()) { 
      std::cout << "SAT in iterative decide" << std::endl;
      
      std::vector<int> *assignment = new std::vector<int>(num_vars);
      
      // todo: find actual assignment
      
      return assignment; 
    }

    while (!BCP()) {
      if (!backtrack()) { 
        std::cout << "UNSAT in iterative BCP" << std::endl;
        return NULL; 
      }
    }
  }

  return NULL;
}

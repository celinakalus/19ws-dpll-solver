#include <vector>
#include <iostream>

#include "trail.h"
#include "config.h"

Trail::Trail(int num_vars) {
  is_assigned.resize(num_vars);
  assignment.resize(num_vars);

  trail_x.clear();
  trail_v.clear();
  trail_b.clear();
}

void Trail::pop(int &x, int &v, int &b) {
  int last = trail_x.size() - 1;

  x = trail_x[last];
  v = trail_v[last];
  b = trail_b[last];

  trail_x.pop_back();
  trail_v.pop_back();
  trail_b.pop_back();

  is_assigned[x] = 0;

#ifdef VERBOSE
  std::cout << "Popping " << x << " of value " << v << " off the trail." << std::endl;
#endif
}

void Trail::push(int x, int v, int b) {
  if (!(is_assigned[x])) {
    trail_x.push_back(x);
    trail_v.push_back(v);
    trail_b.push_back(b);

    is_assigned[x] = 1;
    assignment[x] = v;

#ifdef VERBOSE
    std::cout << "Pushing " << x << " with value " << v << " to the trail." << std::endl;
#endif
  } else {
#ifdef VERBOSE
    std::cout << "Variable " << x << " is pushed to trail, but has already been assigned!" << std::endl;
#endif
  }
}

int Trail::empty() {
  return trail_x.empty();
}

int Trail::variable_assigned(int x) {
  return is_assigned[x];
}

int Trail::variable_value(int x) {
  return assignment[x];
}

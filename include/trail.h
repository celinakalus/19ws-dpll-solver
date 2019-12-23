#ifndef _TRAIL_H_
#define _TRAIL_H_

#include <vector>

class Trail
{
private:
  std::vector<int> is_assigned;
  std::vector<int> assignment;

  std::vector<int> trail_x;
  std::vector<int> trail_v;
  std::vector<int> trail_b;

public:
  Trail(int num_vars);

  void pop(int &x, int &v, int &b);
  void push(int x, int v, int b);
  int empty();

  int variable_assigned(int x);
  int variable_value(int x);
};

#endif

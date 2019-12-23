#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <ratio>
#include <chrono>

#include "dpll.h"
#include "config.h"
#include "trail.h"

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
	os << "[";
	for (auto it = v.begin(); it != v.end(); ++it) {
		if (it != v.begin()) os << ", ";
		os << *it;
	}
	return os << "]";
}

std::vector<std::vector<int>> parse_dimacs(std::string filename) {
	std::vector<std::vector<int>> clauses;
	std::ifstream in(filename);
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		if (line == "") continue;
		if (line.front() == 'c') continue;
		if (line.front() == 'p') continue;
		clauses.emplace_back();
		const char* begin = line.c_str();
		char* end = nullptr;
		while (true) {
			auto id = std::strtoll(begin, &end, 10);
			begin = end;
			if (id == 0) break;
			clauses.back().emplace_back(id);
		}
		if (clauses.back().empty()) clauses.pop_back();
	}
	return clauses;
}

int main(int argc, char* argv[]) {
	using namespace std::chrono;

  assert(argc == 2);
	auto clauses = parse_dimacs(argv[1]);

#ifdef VERBOSE
	//std::cout << clauses << std::endl;
#endif

  int num_vars = -1;
  std::vector<int> assignment;
  
  high_resolution_clock::time_point begin, end;
  
  begin = high_resolution_clock::now();
  int result = solve_dpll(clauses, &num_vars, &assignment);
  end = high_resolution_clock::now();

  duration<double> difftime = duration_cast<duration<double>>(end - begin);

#ifdef VERBOSE
  std::cout.precision(3);
  std::cout << "time: " << std::fixed << difftime.count() << std::endl;
#endif

  if (result) {
    std::cout << "sat " << assignment << std::endl;
    return 10;
  } else {
    std::cout << "unsat" << std::endl;
    return 20;
  }
}

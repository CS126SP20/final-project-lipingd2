//
// Created by Liping Deng on 4/24/20.
//

#ifndef FINALPROJECT_SOLVER_H
#define FINALPROJECT_SOLVER_H
#include <queue>
#include <vector>

using IntPair = std::pair<int, int>;

enum RowOperationType {
  ROW_SWAP,
  ROW_XOR,
};

struct RowOperation {
  RowOperationType a;
  int x;
  int y;
};

class Solver {
  const int n;
  bool** A;
  std::vector<RowOperation> records;
  int rank;
 public:
  explicit Solver(int);
  ~Solver();
  bool solve(const bool* const, std::queue<IntPair>&);
};

#endif  // FINALPROJECT_SOLVER_H

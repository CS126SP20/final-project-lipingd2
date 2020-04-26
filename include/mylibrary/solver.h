//
// Created by Liping Deng on 4/24/20.
//

#ifndef FINALPROJECT_SOLVER_H
#define FINALPROJECT_SOLVER_H
#include <queue>
#include <vector>

using IntPair = std::pair<int, int>;

enum process {
  swapTwo,
  exclusiveOr,
};

struct operation {
  process a;
  int x;
  int y;
};

class solver {
  static const int N = 5;
  bool A[N * N][N * N];
  std::vector<operation> records;
  int rank;
 public:
  solver();
  bool solveMatrix(bool a[], std::queue<IntPair> &b);
};

#endif  // FINALPROJECT_SOLVER_H

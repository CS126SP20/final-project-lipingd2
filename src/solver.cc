//
// Created by Liping Deng on 4/24/20.
//

#include "mylibrary/solver.h"
#include <algorithm>
#include "mylibrary/util.h"

using std::fill_n;
using std::swap;
using std::vector;

// Destructor
Solver::~Solver() {
  for (int i = 0; i < n*n; i++) delete[] A[i];
  delete[] A;
  records.clear();
}

// reduce the matrix given n
Solver::Solver(int _n): n(_n) {
  // allocate memory for A
  A = new bool*[n*n];
  for (int i = 0; i < n*n; i++) {
    A[i] = new bool[n*n];
    fill_n(A[i], n*n, 0);
  }

  // construct matrix for nxn
  for (int i = 0; i < n * n; i++) {
    A[i][i] = 1;
    int x = i / n, y = i % n;
    for (int dir = 0; dir < 4; dir++) {
      int nx = x + dx4[dir], ny = y + dy4[dir];
      if (isInside(nx, n) && isInside(ny, n))
        A[i][n * nx + ny] = 1;
    }
  }

  // Gauss-Jordan Elimination
  int i = 0;
  for (int k = 0; k < n * n; k++) {
    if (A[i][k] != 1) {
       bool foundPivot = false;
       for (int x = i; x < n * n; x++) {
         if (A[x][k] == 1) {
           for (int q = k; q < n * n; q++) {
             swap(A[i][q],A[x][q]);
           }
           records.push_back({ROW_SWAP, i, x});
           foundPivot = true;
           break;
         }
       }
      if (!foundPivot) continue;
    }
    for (int x = 0; x < n * n; x++) {
      if (x != i && A[x][k] == 1) {
        for (int y = k; y < n * n; y++) {
          A[x][y] ^= A[i][y];  // xor is addition in modulo 2
        }
        records.push_back({ROW_XOR, i, x});
      }
    }
    ++i;
  }

  rank = i;
}

// a: current board
// b: operations produced
bool Solver::solve(const bool* const curr, std::queue<IntPair>& q) {
  bool* c = new bool[n*n];
  for (int i = 0; i < n*n; i++) c[i] = !curr[i];
  for (RowOperation record: records) {
    if (record.a == ROW_SWAP) {
      swap(c[record.x], c[record.y]);
    } else {
      c[record.y] ^= c[record.x];
    }
  }
  //Do we have answer?
  // if 00000 1, no answer
  for (int i = rank; i < n * n; i++) {
    if (c[i] == 1) {
      return false;
    }
  }
  // give shortest path
  for (int i = 0, j = 0; i < rank; ++i, ++j) {
    for (; j < n * n && !A[i][j]; ++j);
    if (j == n*n) break;
    if (c[i]) q.push({j / n, j % n});
  }
  return true;
}
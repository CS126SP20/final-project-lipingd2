//
// Created by Liping Deng on 4/24/20.
//

#include "mylibrary/solver.h"
#include <algorithm>
#include "mylibrary/util.h"

using std::fill_n;
using std::swap;
using std::vector;

solver::solver() {
  fill_n(&A[0][0], N * N * N * N, 0);
  for (int i = 0; i < N * N; i++) {
    A[i][i] = 1;
    int x = i / N, y = i % N;
    for (int dir = 0; dir < 4; dir++) {
      int nx = x + dx4[dir], ny = y + dy4[dir];
      if (isInside(nx, N) && isInside(ny, N))
        A[i][N * nx + ny] = 1;
    }
  }

  int i = 0;
  for (int k = 0; k < N * N;) {
    if (A[i][k] != 1) {
       bool foundPivot = false;
       for (int x = i; x < N * N; x++) {
         if (A[x][k] == 1) {
           for (int q = k; q < N * N; q++) {
             swap(A[i][q],A[x][q]);
           }
           records.push_back({swapTwo,i,x});
           foundPivot = true;
           break;
         }
       }
      if (!foundPivot) {
        k++;
        continue;
      }
    }
    for (int x = 0; x < N * N; x++) {
      if (x != i && A[x][k] == 1) {
        for (int y = k; y < N * N; y++) {
          A[x][y] ^= A[i][y];
        }
        records.push_back({exclusiveOr, i, x});
      }
    }
    ++i;
  }
  rank = i;
}

bool solver::solveMatrix(bool a[], std::queue<IntPair>& b) {
  for (operation record: records) {
    if (record.a == swapTwo) {
      swap(a[record.x], a[record.y]);
    } else {
      a[record.y] ^= a[record.x];
    }
  }
  for (int i = rank; i < N * N; i++) {
    if (a[i] == 1) {
      return false;
    }
  }
  for (int i = 0, j = 0; i < rank; ++i, ++j) {
    for (; j < N * N && !A[i][j]; ++j);
    if (j == N * N) break;
    if (a[i]) b.push({j / N, j % N});
  }
  return true;
}
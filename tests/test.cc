// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>
#include "mylibrary/solver.h"
#include <algorithm>
using std::fill_n;

// test for Solver
TEST_CASE("Solve 2 * 2 board", "[solver]") {
  int n = 2;
  Solver solve(n);
  bool c[n * n];
  for (int i = 0; i < n * n; i++) c[i] = 0;
  std::queue<IntPair> q;
  std::queue<IntPair> a;
  for (int i = 0; i < n * n; i++) a.push({i / n, i % n});
  REQUIRE(solve.solve(c,q) == true);
  REQUIRE(a == q);
}

TEST_CASE("Initial board is not empty", "[solver]") {
  int n = 2;
  Solver solve(n);
  bool c[n * n];
  for (int i = 0; i < n * n; i++)  c[i] = 0;
  c[0] = 1;
  std::queue<IntPair> q;
  std::queue<IntPair> a;
  a.push({3 / n, 3 % n});
  REQUIRE(solve.solve(c,q) == true);
  REQUIRE(a == q);
}

TEST_CASE("No solution", "[solver]") {
  int n = 5;
  Solver solver(n);
  bool c[n * n];
  for (int i = 0 ; i < n * n; i++) c[i] = 0;
  c[12] = 1;
  c[13] = 1;
  std::queue<IntPair> q;
  REQUIRE(solver.solve(c,q) == false);
}
//
// Created by Liping Deng on 4/25/20.
//

#include "mylibrary/util.h"
#include <sstream>
using std::istringstream;

bool isInside(int x, int N) {
  return 0 <= x && x < N;
}

vector<string> splitStringBySpace(string s) {
  istringstream iss(s);
  vector<string> ret;
  for (string word; iss >> word; ret.push_back(word));
  return ret;
}
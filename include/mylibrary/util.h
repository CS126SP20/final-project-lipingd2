//
// Created by Liping Deng on 4/25/20.
//

#ifndef FINALPROJECT_UTIL_H
#define FINALPROJECT_UTIL_H
#include <string>
#include <vector>
using std::string;
using std::vector;

bool isInside(int, int);

const int dx4[] = {1, 0, -1, 0};
const int dy4[] = {0, 1, 0, -1};

vector<string> splitStringBySpace(string);

#endif  // FINALPROJECT_UTIL_H

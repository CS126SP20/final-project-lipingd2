//
// Created by Liping Deng on 5/4/20.
//

#ifndef FINALPROJECT_CONF_H
#define FINALPROJECT_CONF_H
#include <string>
#include <optional>
using std::string;

namespace myapp {

struct Conf {
 public:
  const int n;
  bool* const initial_board;

  Conf(int = 0);
  static Conf* GetConfigFromFile(const string&);
  ~Conf();
};

}

#endif  // FINALPROJECT_CONF_H

//
// Created by Liping Deng on 5/4/20.
//

#include "mylibrary/conf.h"
#include "mylibrary/config.h"
#include "toml.h"
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std::string_view_literals;
using std::all_of;
using std::cout;
using std::endl;

namespace myapp {

Conf::Conf(int _n): n(_n), initial_board(new bool[n*n]) {
  std::fill_n(initial_board, n*n, 0);
}

// will return a nullptr when there is error
Conf* Conf::GetConfigFromFile(const string& filename) {
  const char *home = getpwuid(getuid())->pw_dir;
  string filepath = string(home) + kConfigPath + filename;

  try {
    auto parsed = cpptoml::parse_file(filepath);
  } catch (const cpptoml::parse_exception& err) {
    cout << err.what() << endl;
    return nullptr;
  }

  auto parsed = cpptoml::parse_file(filepath);

  // n
  int n = parsed->get_as<int>("n").value_or(kDefN);
  if (!((kMinN <= n && n <= kMaxN))) n = kDefN;
  Conf* conf = new Conf{n};

  // initial_board
  auto init = parsed->get_array_of<int64_t>("init");
  if (init && init->size() == n*n && all_of(init->begin(), init->end(),
      [](int x){return x == 0 || x == 1;})) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        conf->initial_board[i*n+j] = init->at(j*n+i);
      }
    }
  }

  return conf;
}

Conf::~Conf() { delete [] initial_board; }

}
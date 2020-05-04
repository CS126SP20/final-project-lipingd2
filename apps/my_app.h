// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <queue>
#include <chrono>
#include <mylibrary/solver.h>
#include "mylibrary/config.h"
#include "mylibrary/conf.h"
#include <stack>

namespace myapp {

using IntPair = std::pair<int, int>;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::string;
using std::stack;

struct Move {
  int pos;
  bool cheating;
};

class MyApp : public cinder::app::App {

  Conf* conf;  // used in the next reset
  int n;
  int tile_size;
  int cursor_size;

  int n_moves; // basic board variables
  bool* board;
  double* disp_board;
  int cursor_x, cursor_y;
  double disp_x, disp_y;

  Solver* solver;   // solver related
  std::queue<IntPair> solve_moves;
  time_point<system_clock> solve_move_last_update;

  string command_line;       // user types here
  string buffer_line;        // messages go here
  bool inConsole;

  stack<Move> past_moves;
  stack<Move> undos;

  bool cheating;             // toggling a single entry instead of 5

  // when the user types a colon, like in vim
  void startConsole();
  void typeToConsole(char);
  void deleteInConsole();

  // when the user finishes a command, either discard or execute it
  void endConsole(bool);

  // evaluate command_line
  void evalConsole();

  // console actions
  void loadConf(const string&);
  void setVar(const string&, const string&);

  // do and undo
  void do_move(const Move, bool);
  void undo_move();
  void redo_move();

  // toggle a plus shape around cursor_x, cursor_y
  void toggle(Move);

  // cheat
  void solveBoard();

  // reset game according to conf.
  void reset();
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

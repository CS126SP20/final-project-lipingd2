// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <queue>
#include <chrono>
#include <mylibrary/solver.h>

namespace myapp {

using IntPair = std::pair<int, int>;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::milliseconds;
class MyApp : public cinder::app::App {
  static const int N = 5;
  static const int tile_size = 800/N;
  static const int kCursorSize = tile_size/20;
  bool board[N][N];
  int cursor_x, cursor_y;
  double disp_x, disp_y;
  solver answer;
  std::queue<IntPair> cheat_moves;
  time_point<system_clock> cheat_move_last_update;

  // toggle a plus shape around cursor_x, cursor_y
  void toggle5();

  // set all board to 0
  void clearBoard();
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;

  void solveBoard();
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

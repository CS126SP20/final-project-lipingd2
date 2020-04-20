// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>


namespace myapp {

class MyApp : public cinder::app::App {
  static const int N = 5;
  static const int tile_size = 800/N;
  static const int kCursorSize = tile_size/20;
  static const int Ndir4 = 4;
  static const int dx4[];
  static const int dy4[];
  bool isInside(int) const;
  bool board[N][N];
  int cursor_x, cursor_y;

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
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

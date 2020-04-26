// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"
#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <gflags/gflags.h>
#include <rapidcsv.h>
#include "mylibrary/util.h"
#include "mylibrary/solver.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace myapp {

DECLARE_string(file);

using cinder::app::KeyEvent;

MyApp::MyApp() : answer() {
  clearBoard();
}

// a empty board at the beginning if user do not give the file
void MyApp::clearBoard() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) board[i][j] = 0;
  }
}

// setup the board
void MyApp::setup() {
  cursor_x = cursor_y = N/2;
  disp_x = disp_y = cursor_x;
  if (!FLAGS_file.empty()) {
    try {
      rapidcsv::Document doc(FLAGS_file,
          rapidcsv::LabelParams(-1, -1));
      for (int i = 0; i < N;i++) {
        std::vector<int> row = doc.GetRow<int>(i);
        for (int j = 0; j < N; j++) {
          board[i][j] = row.at(j);
        }
      }
    } catch (std::ios_base::failure) {
      std::cout << "File not found or csv error\n";
      exit(1);
    }
  }
}

void MyApp::update() {
  // cheat moves
  static constexpr duration<double> cheat_move_interval = milliseconds(300);
  if (cheat_moves.size()) {
    if (system_clock::now() - cheat_move_last_update > cheat_move_interval) {
      cheat_move_last_update = system_clock::now();
      auto [x, y] = cheat_moves.front();
      cheat_moves.pop();
      cursor_x = x, cursor_y = y;
      toggle5();
    }
  }

  // cursor diplay
  disp_x += ((double)cursor_x - disp_x) * 0.3;
  disp_y += ((double)cursor_y - disp_y) * 0.3;
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();

  // background
  cinder::gl::clear(cinder::Color::black());

  // draw board
  cinder::gl::color(cinder::Color::gray(0.5));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (board[i][j]) {
        cinder::gl::drawSolidRect(cinder::Rectf(
            tile_size * i,
            tile_size * j,
            tile_size * i + tile_size,
            tile_size * j + tile_size));
      }
    }
  }

  // draw cursor
  cinder::gl::color(cinder::Color::white());
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x,
      tile_size * disp_y,
      tile_size * disp_x + tile_size,
      tile_size * disp_y + kCursorSize));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x,
      tile_size * disp_y,
      tile_size * disp_x + kCursorSize,
      tile_size * disp_y + tile_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x + tile_size - kCursorSize,
      tile_size * disp_y,
      tile_size * disp_x + tile_size,
      tile_size * disp_y + tile_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x,
      tile_size * disp_y + tile_size - kCursorSize,
      tile_size * disp_x + tile_size,
      tile_size * disp_y + tile_size));

}

using cinder::app::KeyEvent;
void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
    case KeyEvent::KEY_k:
      if (cursor_y != 0) cursor_y--;
      break;
    case KeyEvent::KEY_DOWN:
    case KeyEvent::KEY_j:
      if (cursor_y != N-1) cursor_y++;
      break;
    case KeyEvent::KEY_LEFT:
    case KeyEvent::KEY_h:
      if (cursor_x != 0) cursor_x--;
      break;
    case KeyEvent::KEY_RIGHT:
    case KeyEvent::KEY_l:
      if (cursor_x != N-1) cursor_x++;
      break;
    case KeyEvent::KEY_SPACE:
      if (cheat_moves.empty()) toggle5();
      break;
    case KeyEvent::KEY_c:
      if (cheat_moves.empty()) solveBoard();
      break;
  }
}
// toggle a "star" around cursor
//  +
// +++
//  +
void MyApp::toggle5() {
  const auto toggle = [&](int x, int y) {
    if (isInside(x, N) && isInside(y, N)) board[x][y] ^= 1;
  };
  toggle(cursor_x, cursor_y);
  for (int dir = 0; dir < 4; dir++) {
    toggle(cursor_x + dx4[dir], cursor_y + dy4[dir]);
  }
}

void MyApp::solveBoard() {
  bool newBoard[N * N];
  for (int i = 0 ; i < N; i++) {
    for (int j = 0; j < N; j++) {
      newBoard[N * i + j] = !board[i][j];
    }
  }
  if (!answer.solveMatrix(newBoard, cheat_moves)) {
    cout << "No Answer\n";
  }
}

}  // namespace myapp

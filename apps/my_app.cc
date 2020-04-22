// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/audio/Voice.h>
#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/Vector.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <rapidcsv.h>
#include <unistd.h>
#include <gflags/gflags.h>
using std::cout;
using std::endl;
using std::string;


namespace myapp {

DEFINE_string(file, "", "the path of a file");

using cinder::app::KeyEvent;

const int MyApp::dx4[] = {1, 0, -1, 0};
const int MyApp::dy4[] = {0, 1, 0, -1};

MyApp::MyApp() {
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

void MyApp::update() { }

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
      tile_size * cursor_x,
      tile_size * cursor_y,
      tile_size * cursor_x + tile_size,
      tile_size * cursor_y + kCursorSize));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * cursor_x,
      tile_size * cursor_y,
      tile_size * cursor_x + kCursorSize,
      tile_size * cursor_y + tile_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * cursor_x + tile_size - kCursorSize,
      tile_size * cursor_y,
      tile_size * cursor_x + tile_size,
      tile_size * cursor_y + tile_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * cursor_x,
      tile_size * cursor_y + tile_size - kCursorSize,
      tile_size * cursor_x + tile_size,
      tile_size * cursor_y + tile_size));
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
      toggle5();
      break;
    case KeyEvent::KEY_c:
      clearBoard();
      break;
  }
}

//to check if it is boundary when do the toggle
bool MyApp::isInside(int x) const {
  return 0 <= x && x < N;
}

// toggle a "star" around cursor
//  +
// +++
//  +
void MyApp::toggle5() {
  const auto toggle = [&](int x, int y) {
    if (isInside(x) && isInside(y)) board[x][y] ^= 1;
  };
  toggle(cursor_x, cursor_y);
  for (int dir = 0; dir < Ndir4; dir++) {
    toggle(cursor_x + dx4[dir], cursor_y + dy4[dir]);
  }
}



}  // namespace myapp

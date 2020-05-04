// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/Font.h>
#include <cinder/Text.h>
#include <cinder/app/App.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <mylibrary/config.h>
#include <vector>
#include <algorithm>
#include <string>

#include "mylibrary/solver.h"
#include "mylibrary/util.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using cinder::app::KeyEvent;
using cinder::ColorA;
using cinder::TextBox;

namespace myapp {

// print a string on screen
const char kNormalFont[] = "Menlo";
template <typename C, typename Alignment>
static void PrintText(const string& text, const C& color, int font_size, const Alignment a,
    const cinder::ivec2& size, const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(a)
      .font(cinder::Font(kNormalFont, font_size))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

MyApp::MyApp(){}

// setup the board
void MyApp::setup() {
  conf = new Conf(kDefN);
  reset();
}

// reset game according to conf.
void MyApp::reset() {
  n = conf->n;

  tile_size = kWidth / n;
  cursor_size = tile_size / 10;

  delete[] board;
  board = new bool[n*n];
  std::copy_n(conf->initial_board, n*n, board);

  delete[] disp_board;
  disp_board = new double[n*n];
  for (int i = 0; i < n*n; i++) disp_board[i] = rand()/RAND_MAX;

  cursor_x = cursor_y = n/2;
  disp_x = n/2, disp_y = n -1;

  delete solver;
  solver = new Solver(n);
  while (solve_moves.size()) solve_moves.pop();
  solve_move_last_update = system_clock::now();

  command_line = "";
  buffer_line = "new game";
  inConsole = false;

  while (past_moves.size()) past_moves.pop();
  while (undos.size()) undos.pop();

  cheating = false;
}

void MyApp::update() {
  // solve moves
  static constexpr duration<double> solve_move_interval = milliseconds(300);
  if (solve_moves.size()) {
    if (system_clock::now() - solve_move_last_update > solve_move_interval) {
      solve_move_last_update = system_clock::now();
      auto [x, y] = solve_moves.front();
      solve_moves.pop();

      cursor_x = x, cursor_y = y;
      do_move({cursor_x * n + cursor_y, false}, true);
    }
  }

  // smoother cursor display
  disp_x += ((double)cursor_x - disp_x) * kCursorSmoother;
  disp_y += ((double)cursor_y - disp_y) * kCursorSmoother;

  // smoother board display
  for (int i = 0; i < n*n; i++) {
    disp_board[i] += ((int)board[i] - disp_board[i]) * kBoardSmoother;
  }

  // win detection
  if (std::all_of(board, board+n*n, [](bool x){return x;})) {
    buffer_line = "You Win!";
  }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();

  // background
  cinder::gl::clear(cinder::Color::black());

  // draw board
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (disp_board[i * n + j] > eps) {
        cinder::gl::color(
            cinder::Color::gray(kBoardMaxGrayValue * disp_board[i * n + j]));
        cinder::gl::drawSolidRect(cinder::Rectf(tile_size * i, tile_size * j,
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
      tile_size * disp_y + cursor_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x,
      tile_size * disp_y,
      tile_size * disp_x + cursor_size,
      tile_size * disp_y + tile_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x + tile_size - cursor_size,
      tile_size * disp_y,
      tile_size * disp_x + tile_size,
      tile_size * disp_y + tile_size));
  cinder::gl::drawSolidRect(cinder::Rectf(
      tile_size * disp_x,
      tile_size * disp_y + tile_size - cursor_size,
      tile_size * disp_x + tile_size,
      tile_size * disp_y + tile_size));

  // draw status
  string status = "#moves: " + std::to_string(past_moves.size());
  PrintText(status, cinder::Color::white(), kStatusFontSize, TextBox::CENTER,
            {kStatusWidth, kBarHeight}, {kStatusX, kHeightNoBar + kBarHeight * 2/3});

  // draw console
  string line = inConsole ? ":" + command_line : buffer_line;
  PrintText(line, cinder::Color::white(), kStatusFontSize, TextBox::LEFT,
            {kWidth - kStatusWidth, kBarHeight},
            {kConsoleX, kHeightNoBar + kBarHeight * 2 / 3});
}

using cinder::app::KeyEvent;
void MyApp::keyDown(KeyEvent event) {
  auto key = event.getCode();

  if (inConsole) {
    // keys accepted when in console
    switch (key) {
      case KeyEvent::KEY_ESCAPE:
        endConsole(false);
        break;
      case KeyEvent::KEY_RETURN:
        endConsole(true);
        break;
      case KeyEvent::KEY_BACKSPACE:
        deleteInConsole();
        break;
      default:
        typeToConsole(key);
    }
  } else {
    // keys accepted when in normal mode
    switch (key) {
      case KeyEvent::KEY_SEMICOLON:
        startConsole();
        break;

        // key accepted only when the solver is not activated
      default:
        if (solve_moves.empty()) {
          switch (key) {
            case KeyEvent::KEY_UP:
            case KeyEvent::KEY_k:
              if (cursor_y != 0) cursor_y--;
              break;
            case KeyEvent::KEY_DOWN:
            case KeyEvent::KEY_j:
              if (cursor_y != n-1) cursor_y++;
              break;
            case KeyEvent::KEY_LEFT:
            case KeyEvent::KEY_h:
              if (cursor_x != 0) cursor_x--;
              break;
            case KeyEvent::KEY_RIGHT:
            case KeyEvent::KEY_l:
              if (cursor_x != n-1) cursor_x++;
              break;
            case KeyEvent::KEY_SPACE:
              do_move({cursor_x * n + cursor_y, cheating}, true);
              break;
            case KeyEvent::KEY_s:
              solveBoard();
              break;
            case KeyEvent::KEY_u:
              undo_move();
              break;
            case KeyEvent::KEY_r:
              redo_move();
            case KeyEvent::KEY_c:
              cheating = !cheating;
          }
        }
    }
  }
}

// command console
// -----------------------------------------------

void MyApp::startConsole() {
  inConsole = true;
  buffer_line = "";
}

void MyApp::endConsole(bool enter) {
  if (enter) evalConsole();
  inConsole = false;
  command_line = "";
}

void MyApp::evalConsole() {
  if (command_line.empty()) return;

  vector<string> split = splitStringBySpace(command_line);
  if (split.empty()) return;
  int nargs = (int)split.size() - 1;

  const string& command = split[0];
  if (command == "e") {            // TODO: toml live loading
    if (nargs == 1) {
      loadConf(split[1]);
    } else {
      buffer_line = ":e <filename>";
    }
  } else if (command == "q") {     // TODO: quit the game
    exit(0);
  } else if (command == "set") {   // TODO: set a variable. eg n
    if (nargs == 2) {
      setVar(split[1], split[2]);
    } else {
      buffer_line = ":set <variable> <value>";
    }
  } else if (command == "reset") { // TODO: live reset
    reset();
  } else {
    buffer_line = "command error";
  }
}

void MyApp::setVar(const string& var, const string& x_str) {
  if (var == "n") {
    int new_n = std::stoi(x_str);
    if (conf->n != new_n && (kMinN <= new_n && new_n <= kMaxN)) {
      delete conf;
      conf = new Conf(new_n);
    } else {
      buffer_line = "invalid n";
    }
  } else {
    buffer_line = "variable '" + var + "' not found";
  }
}

void MyApp::loadConf(const string& filename) {
  Conf* new_conf = Conf::GetConfigFromFile(filename);
  if (new_conf) {
    delete conf;
    conf = new_conf;
    reset();
  } else {
    buffer_line = "loading config failed.";
    delete new_conf;
  }
}

void MyApp::typeToConsole(char c) {
  if (command_line.size() < kMaxCommandLen)
    command_line.push_back(c);
}

void MyApp::deleteInConsole() {
  if (command_line.size()) command_line.pop_back();
}

// board actions
// ----------------------------------------------

void MyApp::do_move(const Move mov, bool clear_undos) {
  past_moves.push(mov);
  toggle(mov);
  if (clear_undos) while (undos.size()) undos.pop();
}

void MyApp::undo_move() {
  if (past_moves.size()) {
    Move mov = past_moves.top();
    past_moves.pop();
    undos.push(mov);

    cursor_x = mov.pos / n, cursor_y = mov.pos % n;
    toggle(mov);
  } else {
    buffer_line = "no move to undo";
  }
}

void MyApp::redo_move() {
  if (undos.size()) {
    Move mov = undos.top();
    undos.pop();

    cursor_x = mov.pos / n, cursor_y = mov.pos % n;
    do_move(mov, false);
  }
};

// toggle a "star" around cursor
//  +
// +++
//  +
void MyApp::toggle(Move mov) {
  const auto toggle1 = [&](int x, int y) {
    if (isInside(x, n) && isInside(y, n)) board[x*n + y] ^= 1;
  };
  toggle1(cursor_x, cursor_y);
  if (!mov.cheating) {
    for (int dir = 0; dir < 4; dir++) {
      toggle1(cursor_x + dx4[dir], cursor_y + dy4[dir]);
    }
  }
}

void MyApp::solveBoard() {
  bool hasSolution = solver->solve(board, solve_moves);
  if (!hasSolution) {
    buffer_line = "no solution :(";
  }
}

}  // namespace myapp

// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <gflags/gflags.h>
#include "my_app.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using cinder::app::App;
using cinder::app::RendererGl;

namespace myapp {
void ParseArgs(vector<string>* args) {
  gflags::SetUsageMessage(
      "Play a game of 5x5. Pass --helpshort for options.");
  int argc = static_cast<int>(args->size());

  vector<char*> argvs;
  for (string& str : *args) {
    argvs.push_back(&str[0]);
  }

  char** argv = argvs.data();
  gflags::ParseCommandLineFlags(&argc, &argv, true);
}

DEFINE_string(file, "", "the path of a csv file");

const int kSamples = 8;
const int kWidth = 800;
const int kHeight = 800;

void SetUp(App::Settings* settings) {
  vector<string> args = settings->getCommandLineArgs();
  ParseArgs(&args);

  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("5x5");
  settings->setResizable(false);
}

}  // namespace myapp

// This is a macro that runs the application.
CINDER_APP(myapp::MyApp,
           RendererGl(RendererGl::Options().msaa(myapp::kSamples)),
           myapp::SetUp)

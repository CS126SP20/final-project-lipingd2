// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <string>
#include "my_app.h"
#include "mylibrary/config.h"

using std::string;
using std::vector;
using cinder::app::App;
using cinder::app::RendererGl;

namespace myapp {

void SetUp(App::Settings* settings) {
  settings->setWindowSize(kWidth, kHeightWithBar);
  settings->setTitle("nxn");
  settings->setResizable(false);
}

}  // namespace myapp

static const int kSamples = 8;
// This is a macro that runs the application.
CINDER_APP(myapp::MyApp,
           RendererGl(RendererGl::Options().msaa(kSamples)),
           myapp::SetUp)

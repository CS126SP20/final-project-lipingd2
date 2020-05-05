//
// Created by Liping Deng on 5/4/20.
//

#ifndef FINALPROJECT_CONFIG_H
#define FINALPROJECT_CONFIG_H

namespace myapp {

const int kWidth = 600;
const int kHeightNoBar = 600;
const int kBarHeight = 30;
const int kHeightWithBar = kHeightNoBar + kBarHeight;
const int kStatusWidth = 90;
const int kStatusX = kWidth - kStatusWidth;
const int kConsoleX = 270;
const int kStatusFontSize = 12;
const double kBoardSmoother = 0.2;
const double kCursorSmoother = 0.3;
const double kBoardMaxGrayValue = 0.5;
const char kConfigPath[] = "/.config/126nxn/";
const int kDefN = 5;
const int kMinN = 2;
const int kMaxN = 15;
const int kMaxCommandLen = 25;
const double eps = 1e-5;
const char kNormalFont[] = "Menlo";
const double kTileScale = 0.1;

}
#endif  // FINALPROJECT_CONFIG_H

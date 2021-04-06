#include "ProgressBar.hh"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <sys/ioctl.h>  //ioctl() and TIOCGWINSZ
#include <unistd.h>     // for STDOUT_FILENO

using namespace std;

namespace {
std::string formatPercent(float value) {
  int a = std::trunc(value);
  int b = std::trunc((value * 10 - a * 10));
  stringstream ss;
  ss << setfill(' ') << setw(3) << a << "." << b;
  return ss.str();
}

struct TerminalSize {
  unsigned int width;
  unsigned int height;
};

// get terminal size
// https://stackoverflow.com/questions/23369503/get-size-of-terminal-window-rows-columns

TerminalSize getTermSize() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  return {size.ws_col, size.ws_row};
}

}  // namespace

namespace dqmcpp {
namespace writers {

ProgressBar::ProgressBar(const int maxSteps) : maxProgressValue(maxSteps){};

void ProgressBar::setMaxValue(const int maxvalue) {
  maxProgressValue = maxvalue;
}

void ProgressBar::update(int value) {
  currentProgress = value;
  draw();
};

void ProgressBar::finish(void) const {
  cout << endl;
}

float ProgressBar::progress() const {
  return static_cast<float>(currentProgress) /
         static_cast<float>(maxProgressValue);
}

void ProgressBar::draw(void) const {
  const auto termwidth = getTermSize().width;
  const unsigned int labelWidth =
      std::max(label.size(), (size_t)termwidth / 12);
  const std::string percents = formatPercent(progress() * 100.0);
  const unsigned int barWidth = termwidth - labelWidth - percents.size() - 5;
  const auto padding = (labelWidth - label.size()) / 2;
  for (unsigned i = 0; i < padding; ++i)
    cout << ' ';
  cout << label;
  for (unsigned i = 0; i < padding; ++i)
    cout << ' ';
  cout << "[";
  const unsigned int pos = barWidth * progress();
  for (unsigned int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << percents << " %\r";
  std::cout.flush();
}

}  // namespace writers
}  // namespace dqmcpp
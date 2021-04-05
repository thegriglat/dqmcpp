#include "ProgressBar.hh"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
std::string formatPercent(float value) {
  int a = std::trunc(value);
  int b = std::trunc((value * 10 - a * 10));
  stringstream ss;
  ss << setfill(' ') << setw(3) << a << "." << b;
  return ss.str();
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
  cout << label;
  for (unsigned i = label.size(); i < labelWidth; ++i)
    cout << ' ';
  cout << "[";
  const int pos = barWidth * progress();
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << formatPercent(progress() * 100.0) << " %\r";
  std::cout.flush();
}

}  // namespace writers
}  // namespace dqmcpp
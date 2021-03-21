#include "ProgressBar.hh"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace {
std::string formatTime(int secs) {
  int minutes = secs / 60;
  int seconds = secs % 60;
  int hours = minutes / 60;
  minutes = minutes % 60;
  stringstream ss;
  ss << setfill('0') << setw(2) << hours << ":" << setfill('0') << setw(2)
     << minutes << ":" << setfill('0') << setw(2) << seconds;
  return ss.str();
}
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

void ProgressBar::finish(void) {
  cout << endl;
}

float ProgressBar::progress() const {
  return static_cast<float>(currentProgress) /
         static_cast<float>(maxProgressValue);
}

// return estimated time in seconds
int ProgressBar::getEstimatedTime() {
  auto _end = std::chrono::steady_clock::now();
  const auto seconds_passed =
      static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                              _end - _beginTime)
                              .count()) /
      1000;
  auto events_passed = currentProgress % updateTimingEvery;
  if (events_passed == 0) {
    events_passed = updateTimingEvery;
  }
  const auto time_per_event = seconds_passed / events_passed;
  int secs = time_per_event * (maxProgressValue - currentProgress);
  secs = std::max(0, secs);
  return secs;
}

void ProgressBar::draw(void) {
  cout << label << " ";
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
  std::cout << "] " << formatPercent(progress() * 100.0) << " % "
            << formatTime(getEstimatedTime()) << "\r";
  std::cout.flush();
}

}  // namespace writers
}  // namespace dqmcpp
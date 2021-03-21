#ifndef PROGRESSBAR_HH

#include <chrono>
#include <cmath>
#include <string>

namespace dqmcpp {
namespace writers {

class ProgressBar {
 private:
  static constexpr int barWidth = 70;
  int updateTimingEvery = 20;
  int currentProgress = 0;
  int maxProgressValue = 1;
  std::string label = "";
  std::chrono::steady_clock::time_point _beginTime;
  void draw(void) const;
  void finish(void) const;
  float progress() const;
  int getEstimatedTime() const;

 public:
  ProgressBar(const int maxSteps = 1)
      : maxProgressValue(maxSteps),
        _beginTime(std::chrono::steady_clock::now()) {
    updateTimingEvery = (maxSteps > 1) ? std::round(std::log(maxSteps)) : 1;
  };

  inline void update(int value) {
    currentProgress = value;
    draw();
    if (currentProgress % updateTimingEvery == 0) {
      _beginTime = std::chrono::steady_clock::now();
    }
  };

  inline void increment(const int increment = 1.0) {
    update(currentProgress + increment);
  };

  inline void setMaxValue(const int maxvalue) { maxProgressValue = maxvalue; }

  inline void setLabel(const std::string& newlabel) { label = newlabel; }

  ~ProgressBar() { finish(); };
};
}  // namespace writers
}  // namespace dqmcpp

#define PROGRESSBAR_HH
#endif  // !PROGRESSBAR_HH
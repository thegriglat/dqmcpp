#ifndef PROGRESSBAR_HH

#include <string>

namespace dqmcpp {
namespace writers {

class ProgressBar {
 private:
  static constexpr int barWidth = 70;
  int currentProgress = 0;
  int maxProgressValue = 1;
  std::string label = "";
  void draw(void);
  void finish(void);

 public:
  ProgressBar(const int maxSteps = 1) : maxProgressValue(maxSteps){};
  inline void Update(int value) {
    currentProgress = value;
    draw();
  };
  inline void Increment(const int increment = 1.0) {
    Update(currentProgress + increment);
  };
  inline void setMaxValue(const int maxvalue) { maxProgressValue = maxvalue; }
  inline void setLabel(const std::string& newlabel) { label = newlabel; }
  ~ProgressBar() { finish(); };
};
}  // namespace writers
}  // namespace dqmcpp

#define PROGRESSBAR_HH
#endif  // !PROGRESSBAR_HH
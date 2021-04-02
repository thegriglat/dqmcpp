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
  void draw(void) const;
  void finish(void) const;
  float progress() const;
  void update(int value);

 public:
  ProgressBar(const int maxSteps = 1);

  inline void increment(const int increment = 1.0) {
    update(currentProgress + increment);
  };

  void setMaxValue(const int maxvalue);
  inline void setLabel(const std::string& newlabel) {
    label = newlabel;
    draw();
  }

  ~ProgressBar() { finish(); };
};
}  // namespace writers
}  // namespace dqmcpp

#define PROGRESSBAR_HH
#endif  // !PROGRESSBAR_HH
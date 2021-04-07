#ifndef PROGRESSBAR_HH

#include <sstream>
#include <string>

namespace dqmcpp {
namespace writers {

class ProgressBar {
 private:
  int currentProgress = 0;
  int maxProgressValue = 1;
  std::string label = "";
  void draw(void) const;
  float progress() const;
  void update(int value);

 public:
  ProgressBar(const int maxSteps = 1);
  void finish(void) const;
  inline void increment(const int increment = 1.0) {
    update(currentProgress + increment);
  };

  void setMaxValue(const int maxvalue);
  inline void setLabel(const std::string& newlabel) {
    label = newlabel;
    draw();
  }

  inline void setLabel(const int run) { setLabel(std::to_string(run)); }
  template <typename T>
  inline void setLabel(const T value) {
    std::stringstream ss;
    ss << value;
    setLabel(ss.str());
  }

  ~ProgressBar() { finish(); };
};
}  // namespace writers
}  // namespace dqmcpp

#define PROGRESSBAR_HH
#endif  // !PROGRESSBAR_HH
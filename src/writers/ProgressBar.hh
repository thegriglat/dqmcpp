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
  void increment(const int increment = 1.0);
  void setMaxValue(const int maxvalue);
  void setLabel(const std::string& newlabel);
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
#ifndef PROGRESSBAR_HH

#include <sstream>
#include <string>

namespace dqmcpp {
namespace writers {

/**
 * @brief Simple progress bar class
 *
 */
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
  /**
   * @brief Increment current counter by increment
   *
   * @param increment
   */
  void increment(const int increment = 1.0);
  /**
   * @brief Get the max value
   *
   * @return int
   */
  inline int getMaxValue() const { return maxProgressValue; };
  /**
   * @brief Set the max value
   *
   * @param maxvalue
   */
  void setMaxValue(const int maxvalue);
  /**
   * @brief Set the label
   *
   * @param newlabel
   */
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
#ifndef GNUPLOT_HH

#include <string>

namespace dqmcpp {
namespace writers {

/**
 * @brief Gnuplot axis class
 *
 */
struct Axis {
  std::string min;
  std::string max;
  Axis(const std::string& min, const std::string& max) : min(min), max(max){};
  Axis(const double min, const double max)
      : min(std::to_string(min)), max(std::to_string(max)){};
  inline double mind() const { return std::atof(min.c_str()); }
  inline double maxd() const { return std::atof(max.c_str()); }
};

/**
 * @brief Base gnuplot wrapper class
 *
 */
class Gnuplot {
 private:
  Axis _xaxis = {"*", "*"};
  Axis _yaxis = {"*", "*"};
  std::string _output = "output";
  std::string _title = "";

 public:
  /**
   * @brief Set the output path for gnuplot
   *
   * @param outfile
   */
  inline void setOutput(const std::string& outfile) { _output = outfile; }
  /**
   * @brief Get the output path
   *
   * @return std::string
   */
  inline std::string getOutput(void) const { return _output; }
  /**
   * @brief Set the plot title
   *
   * @param title
   */
  inline void setTitle(const std::string& title) { _title = title; }
  /**
   * @brief Get the plot itle
   *
   * @return std::string
   */
  inline std::string getTitle(void) const { return _title; }
  /**
   * @brief Set gnuplot xrange
   *
   * @param min number or "*"
   * @param max number or "*"
   */
  inline void setX(const std::string& min, const std::string& max) {
    _xaxis = {min, max};
  }
  /**
   * @brief Set gnuplot axis
   *
   * @param x Axis object
   */
  inline void setX(const Axis& x) { _xaxis = x; };
  /**
   * @brief Get current axis settings
   *
   * @return Axis
   */
  inline Axis getX() const { return _xaxis; };
  inline void setY(const std::string& min, const std::string& max) {
    _yaxis = {min, max};
  }
  inline void setY(const Axis& y) { _yaxis = y; };
  Axis axis(const double min, const double max);

  inline void setX(const double min, const double max) {
    _xaxis = axis(min, max);
  }
  inline void setY(const double min, const double max) {
    _yaxis = axis(min, max);
  }

  inline Axis getY() const { return _yaxis; };
};

}  // namespace writers
}  // namespace dqmcpp

#define GNUPLOT_HH
#endif  // !GNUPLOT_HH

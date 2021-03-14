#ifndef GNUPLOT_HH

namespace dqmcpp {
namespace writers {

struct Axis {
  double min;
  double max;
};

class Gnuplot {
 private:
  Axis _xaxis;
  Axis _yaxis;
  std::string _output = "output";
  std::string _title = "title";

 public:
  inline void setOutput(const std::string outfile) { _output = outfile; }
  inline std::string getOutput(void) const { return _output; }
  inline void setTitle(const std::string title) { _title = title; }
  inline std::string getTitle(void) const { return _title; }
  inline void setX(const double min, const double max) {
    _xaxis = {std::min(min, max), std::max(min, max)};
  }
  inline Axis getX() const { return _xaxis; };
  inline void setY(const double min, const double max) {
    _yaxis = {std::min(min, max), std::max(min, max)};
  }
  inline Axis getY() const { return _yaxis; };
};

}  // namespace writers
}  // namespace dqmcpp

#define GNUPLOT_HH
#endif  // !GNUPLOT_HH
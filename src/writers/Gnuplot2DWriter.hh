#ifndef GNUPLOT2DWRITER_HH
/**
 * @file Gnuplot2DWriter.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Output 2D histograms
 */
#include <functional>
#include <map>
#include <ostream>
#include <vector>
#include "Gnuplot2D.hh"

namespace dqmcpp {
namespace writers {

class Gnuplot2DWriter : public Gnuplot2D {
 public:
  struct Rectangle {
    unsigned int xbin;
    unsigned int ybin;
    unsigned int pattern = 5;
    Rectangle(const unsigned int _xbin,
              const unsigned int _ybin,
              const unsigned int _pattern = 5)
        : xbin(_xbin), ybin(_ybin), pattern(_pattern){};
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& r);
  };

 private:
  const Data2D* _data = nullptr;
  std::vector<std::string> _xlabels;
  std::vector<std::string> _ylabels;
  std::function<bool(const std::string&, const std::string&)> sortfn = nullptr;
  std::vector<Rectangle> boxes;
  inline unsigned int nrows() const { return _xlabels.size(); }
  inline unsigned int ncolumns() const { return _ylabels.size(); }

  inline double get(const std::string& x, const std::string& y) const {
    return _data->at({x, y});
  }

 public:
  Gnuplot2DWriter(const Data2D& data);
  void setSortYFn(std::function<bool(const std::string&, const std::string&)>
                      sort_function);
  void setXlabels(const std::vector<std::string>& xlabels);
  void addBox(const std::string& xlabel,
              const std::string& ylabel,
              const unsigned int pattern = 5);
  inline void addBox(const std::pair<std::string, std::string>& labels,
                     const unsigned int pattern = 5) {
    addBox(labels.first, labels.second, pattern);
  }

  friend std::ostream& operator<<(std::ostream& os, const Gnuplot2DWriter& gw);
};

}  // namespace writers
}  // namespace dqmcpp
#define GNUPLOT2DWRITER_HH
#endif
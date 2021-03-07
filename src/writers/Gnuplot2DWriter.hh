#ifndef GNUPLOT2DWRITER_HH

#include <functional>
#include <iostream>
#include <map>
#include <vector>

class Gnuplot2DWriter {
  using Data2D = std::map<std::pair<std::string, std::string>, double>;

 private:
  Data2D _data;
  std::vector<std::string> _xlabels;
  std::vector<std::string> _ylabels;
  inline unsigned int nrows() const { return _xlabels.size(); }
  inline unsigned int ncolumns() const { return _ylabels.size(); }

  inline double get(const std::string& x, const std::string& y) const {
    return _data.at({x, y});
  }

 public:
  Gnuplot2DWriter(Data2D& data);
  friend std::ostream& operator<<(std::ostream& os, const Gnuplot2DWriter& gw);
};

#define GNUPLOT2DWRITER_HH
#endif
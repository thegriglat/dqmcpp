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
#include "../ECAL/ECAL.hh"
#include "../common/common.hh"
#include "Gnuplot2D.hh"

namespace dqmcpp {
namespace writers {

/**
 * @brief Write two-dimensional gnuplot plot
 *
 */
class Gnuplot2DWriter : public Gnuplot2D {
 public:
  /**
   * @brief Class to draw rectangle on plot
   *
   */
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
  std::function<bool(const std::string&, const std::string&)> sortfn =
      std::less<std::string>{};
  std::vector<Rectangle> boxes;
  inline unsigned int nrows() const { return _xlabels.size(); }
  inline unsigned int ncolumns() const { return _ylabels.size(); }

  inline double get(const std::string& x, const std::string& y) const {
    return _data->at({x, y});
  }

 public:
  Gnuplot2DWriter(const Data2D& data);
  /**
   * @brief Set Y label sorting function
   *
   * @param sort_function Sorting function (string, string) -> bool
   */
  void setSortYFn(std::function<bool(const std::string&, const std::string&)>
                      sort_function);
  /**
   * @brief Explicitly set x labels vector
   *
   * @param xlabels
   */
  void setXlabels(const std::vector<std::string>& xlabels);

  /**
   * @brief Explicitly set x labels vector from run's vector
   *
   * @param runlist ECAL::Run vector
   */
  void setXlabels(const std::vector<ECAL::Run>& runlist);

  /**
   * @brief Explicitly set x labels vector for any type
   *
   * @tparam T
   * @tparam BinaryOp
   * @param list input vector
   * @param op convert function (T)->std::string
   */
  template <typename T, typename BinaryOp>
  void setXlabels(const std::vector<T>& list, BinaryOp op) {
    setXlabels(common::map<T, std::string>(list, op));
  }

  /**
   * @brief Explicitly set x labels vector for ECAL::RunData<T> type
   *
   * @tparam T ECAL::RunData template parameter
   * @param rdlist input vector
   */
  template <typename T>
  void setXlabels(const std::vector<ECAL::RunData<T>>& rdlist) {
    setXlabels(rdlist, [](const ECAL::RunData<T>& rd) {
      return std::to_string(rd.run.runnumber);
    });
  }

  /**
   * @brief Add box in cell [xlabel, ylabel] with draw 'pattern'
   *
   * @param xlabel x coordinate
   * @param ylabel y coordinate
   * @param pattern gnuplot box draw pattern
   */
  void addBox(const std::string& xlabel,
              const std::string& ylabel,
              const unsigned int pattern = 5);
  /**
   * @brief Add boxes in input coordinates
   *
   * @param labels box coordinate
   * @param pattern gnuplot draw pattern
   */
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
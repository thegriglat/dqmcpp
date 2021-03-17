#ifndef DQMCPPSYSTEM_HH

#include <fstream>
#include <string>
#include <vector>

namespace dqmcpp {
namespace common {
namespace gnuplot {

/**
 * @brief Gets output of gnuplot commands
 *
 * @param content gnuplot script content
 * @return std::string
 */
std::string exec(const std::string& content);

struct FitParameter {
  std::string name;
  double value = 1.0;
  FitParameter(const std::string& parameter_name,
               const double initialValue = 1.0)
      : name(parameter_name), value(initialValue){};
  inline friend std::ostream& operator<<(std::ostream& os,
                                         const FitParameter& fp) {
    os << fp.name << " fitted to " << fp.value;
    return os;
  }
};

using FitParameters = std::vector<FitParameter>;
struct Fit {
  std::string equation;
  FitParameters parameters;
  Fit(const std::string equation, const FitParameters& parameters)
      : equation(equation), parameters(parameters){};
  inline friend std::ostream& operator<<(std::ostream& os, const Fit& fp) {
    os << "fit: " << fp.equation << std::endl;
    for (auto& p : fp.parameters)
      os << "  " << p << std::endl;
    return os;
  }
  const FitParameter& getParameter(const std::string& name) const;
};

/**
 * @brief Fit x/y data with Fit structure.
 *
 * @param x x coord
 * @param y y coord
 * @param fit Fit structure
 * @return void
 */
Fit fit(const std::vector<double>& x,
        const std::vector<double>& y,
        const Fit& fit);

/**
 * @brief Fit with function which should returs std::pair of x/x
 *
 * @tparam T
 * @tparam BinaryOp
 * @param list
 * @param op
 * @param fit
 * @return Fit
 */
template <typename T, typename BinaryOp>
Fit fit(const std::vector<T>& list, BinaryOp op, const Fit& initial_fit) {
  std::vector<double> _x;
  _x.reserve(list.size());
  std::vector<double> _y;
  _y.reserve(list.size());
  for (auto& elem : list) {
    std::pair<double, double> pair = op(elem);
    _x.push_back(pair.first);
    _y.push_back(pair.second);
  };
  return fit(_x, _y, initial_fit);
}

/**
 * @brief Returns gauss fit function
 *
 * @param a default value for a
 * @param mu default value for mu
 * @param sigma default value for sigma
 * @return Fit
 */
Fit gauss(double a = 1, double mu = 1, double sigma = 1);

}  // namespace gnuplot
}  // namespace common
}  // namespace dqmcpp

#define DQMCPPSYSTEM_HH
#endif
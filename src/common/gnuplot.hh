#ifndef DQMCPPSYSTEM_HH

#include <fstream>
#include <sstream>
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

template <typename T>
std::string exec(const T& t) {
  std::stringstream ss;
  ss << t;
  return exec(ss.str());
}

/**
 * @brief Struct to keep fit parameter, name and value
 *
 */
struct FitParameter {
  std::string name;
  double value = 1.0;
  FitParameter(const std::string& parameter_name,
               const double initialValue = 1.0)
      : name(parameter_name), value(initialValue){};
};

using FitParameters = std::vector<FitParameter>;
/**
 * @brief Class to match equation and FitParameter. It's used as input object
 * for fitting.
 *
 */
struct Fit {
  std::string equation;
  FitParameters parameters;
  Fit(const std::string& equation, const FitParameters& parameters)
      : equation(equation), parameters(parameters){};
  const FitParameter& getParameter(const std::string& name) const;
};

/**
 * @brief Fit x/y data with Fit structure.
 *
 * @param x x coordinate
 * @param y y coordinate
 * @param fit Fit object
 * @return Fit
 */
Fit fit(const std::vector<double>& x,
        const std::vector<double>& y,
        const Fit& fit);

/**
 * @brief Fit with function which should returs std::pair of x/x
 *
 * @tparam T data type
 * @tparam BinaryOp Function (T) -> std::pair<double,double> of coordinates
 * @param list input list
 * @param op convert function
 * @param fit Fit object
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

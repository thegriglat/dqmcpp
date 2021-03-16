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
    os << fp.name << " ~> " << fp.value;
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
};

/**
 * @brief Fit x/y data with Fit structure. Change parameters value in-place
 *
 * @param x x coord
 * @param y y coord
 * @param fit Fit structure
 * @return void
 */
Fit fit(const std::vector<double>& x,
        const std::vector<double>& y,
        const Fit& fit);

}  // namespace gnuplot
}  // namespace common
}  // namespace dqmcpp

#define DQMCPPSYSTEM_HH
#endif
#include "ProgressBar.hh"

#include <iostream>

using namespace std;

namespace dqmcpp {
namespace writers {

void ProgressBar::finish(void) {
  cout << endl;
}

void ProgressBar::draw(void) {
  cout << label << " ";
  cout << "[";
  const float progress = static_cast<float>(currentProgress) /
                         static_cast<float>(maxProgressValue);
  //   cout << "progress " << progress << " cp " << currentProgress << " mp "
  //    << maxProgressValue << endl;
  const int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";
  std::cout.flush();
}

}  // namespace writers
}  // namespace dqmcpp
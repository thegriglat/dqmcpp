#ifndef TTF4OCCUPANCY_HH

#include <vector>
#include "../dataclasses/ECAL.hh"
#include "Plugin.hh"

class TTF4Occupancy : public Plugin {
 private:
  std::vector<ECAL::TTRunData> readTT();

 public:
  void Process();
};

#define TTF4OCCUPANCY_HH
#endif  // !TTF4OCCUPANCY_HH
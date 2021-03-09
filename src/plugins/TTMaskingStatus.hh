#ifndef TTF4MASKINGSTATUS_HH

#include <vector>
#include "Plugin.hh"

struct TTData {
  // TT number
  int tt;
  // EB = 0 ; EE+ = 1; EE- = -1;
  int iz;
  double value;
  TTData(int ttnum, int z, double val) : tt(ttnum), iz(z), value(val){};
  inline friend bool operator>(const TTData& a, const TTData& b) {
    return a.value > b.value;
  };
  inline friend std::ostream& operator<<(std::ostream& os, const TTData& elem) {
    os << "tt[tt=" << elem.tt << ",iz=" << elem.iz << "] = " << elem.value;
    return os;
  }
};

struct TTRunData {
  int run;
  std::vector<TTData> ttdata;
  TTRunData(int _run, const std::vector<TTData>& ttd)
      : run(_run), ttdata(ttd){};
};

class TTMaskingStatus : public Plugin {
 private:
  std::vector<TTRunData> Init(void);

 public:
  void Process();
  std::vector<TTRunData> get(void);
};

#define TTF4MASKINGSTATUS_HH
#endif

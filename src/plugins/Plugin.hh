#ifndef PLUGIN_HH

#include <string>
#include <vector>
#include "../dataclasses/ECAL.hh"
#include "../readers/Reader.hh"
#include "../readers/RunListReader.hh"

/**
 * @brief Virtual class for plugins (tests)
 *
 */
class Plugin {
 protected:
  Reader* reader = nullptr;
  RunListReader* runListReader = nullptr;

 public:
  /**
   * @brief Main function for Plugin
   *
   */
  virtual void Process(void) = 0;
  /**
   * @brief Set the Reader object
   *
   * @param UrlReader Reader* pointer
   */
  inline void setReader(Reader* UrlReader) { reader = UrlReader; }
  /**
   * @brief Set the Run List Reader object
   *
   * @param runlistreader RunListReader* pointer
   */
  inline void setRunListReader(RunListReader* runlistreader) {
    runListReader = runlistreader;
  }
};

#define PLUGIN_HH
#endif
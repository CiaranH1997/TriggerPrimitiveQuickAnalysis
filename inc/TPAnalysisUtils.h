#ifndef INC_TPANAUTILS_H_
#define INC_TPANAUTILS_H_

#include <iostream>
#include <vector>
#include <string>
#include <memory>  // For std::unique_ptr

#include <TFile.h>
#include <TTree.h>

#include "Neutrino.h"
#include "Event.h"

std::vector<Neutrino> LoadNuAnaFiles(std::string filename);

std::vector<std::unique_ptr<Event>> LoadTPFiles(std::string nufiles, int file_type);

namespace cut {

  //void TimeFilterAlg(double const &time_window, std::vector<int> &max_adc_sum, std::vector<int> &max_tp_multiplicity, std::vector<std::unique_ptr<Event>> tpc_events);
  //bool TimeFilterAlg(double const &adc_cut, double const &time_window, std::unique_ptr<Event> &tpc_event);
  bool TimeFilterAlg(double const &adc_cut, double const &time_window, std::vector<int> &max_adc_sum, std::vector<int> &max_tp_multiplicity, std::unique_ptr<Event> &tpc_event);


}

#endif

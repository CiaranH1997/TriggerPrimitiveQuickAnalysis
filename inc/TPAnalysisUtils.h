#ifndef INC_TPANAUTILS_H_
#define INC_TPANAUTILS_H_

#include <iostream>
#include <vector>
#include <string>
#include <memory>  // For std::unique_ptr

#include "Neutrino.h"
#include "Event.h"
#include "TPCEvents.h"

std::vector<Neutrino> LoadNuAnaFiles(std::string filename);

std::vector<std::unique_ptr<Event>> LoadTPFiles(std::string nufiles, int file_type, double &drift_window);

void CalcProbPassADCIntThreshold(double const &weight_numu, double const &weight_nue, double const &drift_window, std::vector<double> const &ADCCuts, 
                                 std::vector<double> &ADCCutEff_cos, std::vector<double> &ADCCutEff_nu_cos, TPCEvents &numu_tpc_events, 
                                 TPCEvents &nue_tpc_events, TPCEvents &cos_tpc_events, int const &alg_type = 0);

void ApplyADCIntegralThreshold(std::vector<int> &v_adc_sum, std::vector<int> &v_adc_sum_events_cut, std::vector<int> &v_tp_multiplicity, double &ADC_SUM_CUT, 
                               double &drift_window, std::vector<std::pair<int, bool>> &adc_int_cut_events, TPCEvents &obj_tpc_events);

void ApplyTimeDataFilter(std::vector<int> &v_time_max_adc_sum, std::vector<int> &v_time_max_adc_sum_events_cut, std::vector<int> &v_time_max_tp_multiplicity, 
                         double &time_filter_ADC_CUT, double &time_filter_window, std::vector<std::pair<int, bool>> &time_filter_events, TPCEvents &obj_tpc_events);

namespace cut {

  // Time filter for offline
  bool TimeFilterAlg(double const &adc_cut, double const &time_window, std::vector<int> &max_adc_sum, std::vector<int> &max_adc_sum_events_cut, 
                     std::vector<int> &max_tp_multiplicity, std::unique_ptr<Event> &tpc_event);
  // ADC Integral Sum cut for online trigger
  bool ADCIntegralSumCut(double const &adc_cut, double const &time_window, std::vector<int> &adc_sum, 
                         std::vector<int> &adc_sum_events_cut, std::vector<int> &tp_multiplicity, std::unique_ptr<Event> &tpc_event);

}

#endif

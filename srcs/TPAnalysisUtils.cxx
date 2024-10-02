#include "TPAnalysisUtils.h"
#include "NeutrinoEvent.h"
#include "CosmicEvent.h"

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>

#include <deque>

//------------------------------------
std::vector<std::vector<TP>> sortByEvent(std::vector<TP> event_tps) {
  int max_event(0);
  for (const auto tp : event_tps) {
    if (tp.event > max_event) max_event = tp.event;
  }

  std::vector<std::vector<TP>> tps_per_event;
  tps_per_event.resize(max_event);

  for (int ev = 0; ev < (int)tps_per_event.size(); ev++) {
    std::vector<TP> tps;
    for (const auto tp : event_tps) {
      if (tp.event == ev+1) tps.push_back(tp);
    }
    tps_per_event.at(ev) = tps;
  }
  return tps_per_event;
}

//------------------------------------
std::vector<Neutrino> LoadNuAnaFiles(std::string filename) {

  // Open the ROOT file
  TFile *nufile = TFile::Open(filename.c_str(), "READ");
  if (!nufile || nufile->IsZombie()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    exit(1);
  }

  // Get the TTree
  TTree *nutree = nullptr;
  nufile->GetObject("ana/tree", nutree);
  if (!nutree) {
    std::cerr << "Error: TTree 'tree' not found in file: " << filename << std::endl;
    nufile->Close();
    exit(1);
  }

  // Set branch addresses
  unsigned int nuEvent, TPCID; 
  int PDG, CCNC; 
  double Energy;
  double nuVertexX, nuVertexY, nuVertexZ;
  nutree->SetBranchAddress("eventID", &nuEvent);
  nutree->SetBranchAddress("TPCID", &TPCID);
  nutree->SetBranchAddress("PDG", &PDG);
  nutree->SetBranchAddress("CCNC", &CCNC);
  nutree->SetBranchAddress("E", &Energy);
  nutree->SetBranchAddress("nuVertexX", &nuVertexX);
  nutree->SetBranchAddress("nuVertexY", &nuVertexY);
  nutree->SetBranchAddress("nuVertexZ", &nuVertexZ);

  std::vector<Neutrino> v_nu_ret;

  // Loop over the entries
  Long64_t nEntriesNu = nutree->GetEntries();
  for (Long64_t i = 0; i < nEntriesNu; ++i) {
    nutree->GetEntry(i);
    Neutrino nu = {(int)nuEvent, Energy, (int)TPCID, PDG, CCNC};
    v_nu_ret.push_back(nu);
  }

  return v_nu_ret;
}

//------------------------------------
std::vector<std::unique_ptr<Event>> LoadTPFiles(std::string tpfile, int file_type, double &drift_window) {

  TFile *file = TFile::Open(tpfile.c_str(), "READ");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file: " << tpfile << std::endl;
    exit(1);
  }
  std::cout << "Opening file: " << tpfile << std::endl;

  // Get the TTree
  TTree *tree = nullptr;
  file->GetObject("offlineTriggerTPCInfoDisplay/TPTree", tree);
  if (!tree) {
    std::cerr << "Error: TTree 'TPTree' not found in file: " << tpfile << std::endl;
    file->Close();
    exit(1);
  }      
   
  // Set branch addresses
  int tpEvent, ROP_ID;
  unsigned int ChannelID, ADC_integral;
  unsigned short ADC_peak;
  long Start_time, Time_over_threshold, Time_peak;
            
  tree->SetBranchAddress("Event", &tpEvent);
  tree->SetBranchAddress("ROP_ID", &ROP_ID);
  tree->SetBranchAddress("ChannelID", &ChannelID);
  tree->SetBranchAddress("ADC_integral", &ADC_integral);
  tree->SetBranchAddress("ADC_peak", &ADC_peak);
  tree->SetBranchAddress("Start_time", &Start_time);
  tree->SetBranchAddress("Time_over_threshold", &Time_over_threshold);  
  tree->SetBranchAddress("Time_peak", &Time_peak);

  std::vector<TP> v_TP;

  // Loop over the entries
  Long64_t nEntries = tree->GetEntries();
	
  for (Long64_t i = 0; i < nEntries; ++i) {
    tree->GetEntry(i);
    TP event_tp = {ChannelID, ROP_ID, Time_peak, ADC_integral, tpEvent};
    // Neutrino time peak offset from zero by ~17.75k ticks
    if (file_type == 1) {
      if ((Time_peak - 17.75e3) <= drift_window) {
        v_TP.push_back(event_tp);
      }
    } else if (file_type == 0) { 
      if (Time_peak <= drift_window) {
        v_TP.push_back(event_tp);
      }
    } else {
      std::cout << "[ERROR] unknown file type" << std::endl;
      exit(1);
    }
  }
  std::cout << "Sort by event." << std::endl;
  auto v_v_TPs = sortByEvent(v_TP);
  std::cout << "Events sorted." << std::endl;

  std::vector<std::unique_ptr<Event>> ret;

  std::cout << "Make vector of events." << std::endl;
  int ev_num(1);
  for (const auto &ev : v_v_TPs) {
    if (file_type == 0) { // Cosmic Events
      ret.emplace_back(std::make_unique<CosmicEvent>(ev_num, ev));
    } else if (file_type == 1) {
      ret.emplace_back(std::make_unique<NeutrinoEvent>(ev_num, ev));
    } else {
      std::cout << "[ERROR] Unknown file type!" << std::endl;
      exit(1);
    }
    ev_num++;
  }

  return ret;
}

//------------------------------------
void CalcProbPassADCIntThreshold(double const &weight_numu, double const &weight_nue, double const &drift_window, std::vector<double> const &ADCCuts, 
                                 std::vector<double> &ADCCutEff_cos, std::vector<double> &ADCCutEff_nu_cos, TPCEvents &numu_tpc_events, 
                                 TPCEvents &nue_tpc_events, TPCEvents &cos_tpc_events, int const &alg_type) {
  TH1D *tempADCInt_adcsum_cos = new TH1D("temp1", "ADC Integral Sum Distribution" , 50, 0, 25e6);
  tempADCInt_adcsum_cos->SetDirectory(nullptr);
  TH1D *tempADCInt_adcsum_nu_cos = new TH1D("temp2", "ADC Integral Sum Distribution" , 50, 0, 25e6);
  tempADCInt_adcsum_nu_cos->SetDirectory(nullptr);

  std::vector<int> v_numu_adc_sum, v_nue_adc_sum, v_cos_adc_sum;
  std::vector<int> v_numu_adc_sum_events_cut, v_nue_adc_sum_events_cut, v_cos_adc_sum_events_cut;
  std::vector<int> v_numu_tp_multiplicity, v_nue_tp_multiplicity, v_cos_tp_multiplicity;

  double null_cut(0); // for total weighted number of events
  for (auto &event : numu_tpc_events.GetTPCEvents()) {
    if (event) {
      if (alg_type == 0) {
        bool pass_adcint_cut = cut::ADCIntegralSumCut(null_cut, drift_window, v_numu_adc_sum, v_numu_adc_sum_events_cut, v_numu_tp_multiplicity, event);
      } else if (alg_type == 1) {
        bool pass_adcint_cut = cut::TimeFilterAlg(null_cut, drift_window, v_numu_adc_sum, v_numu_adc_sum_events_cut, v_numu_tp_multiplicity, event);
      }
    }
  }
  for (auto &event : nue_tpc_events.GetTPCEvents()) {
    if (event) {
      if (alg_type == 0) {
        bool pass_adcint_cut = cut::ADCIntegralSumCut(null_cut, drift_window, v_nue_adc_sum, v_nue_adc_sum_events_cut, v_nue_tp_multiplicity, event);
      } else if (alg_type == 1) {
        bool pass_adcint_cut = cut::TimeFilterAlg(null_cut, drift_window, v_nue_adc_sum, v_nue_adc_sum_events_cut, v_nue_tp_multiplicity, event);
      }
    }
  }
  for (auto &event : cos_tpc_events.GetTPCEvents()) {
    if (event) {
      if (alg_type == 0) {
        bool pass_adcint_cut = cut::ADCIntegralSumCut(null_cut, drift_window, v_cos_adc_sum, v_cos_adc_sum_events_cut, v_cos_tp_multiplicity, event);
      } else if (alg_type == 1) {
        bool pass_adcint_cut = cut::TimeFilterAlg(null_cut, drift_window, v_cos_adc_sum, v_cos_adc_sum_events_cut, v_cos_tp_multiplicity, event);
      }
    }
  }

  for (const auto adc_sum : v_numu_adc_sum) {
    tempADCInt_adcsum_nu_cos->Fill((double)adc_sum, weight_numu);
  }
  for (const auto adc_sum : v_nue_adc_sum) {
    tempADCInt_adcsum_nu_cos->Fill((double)adc_sum, weight_nue);
  }
  for (const auto adc_sum : v_cos_adc_sum) {
    tempADCInt_adcsum_cos->Fill((double)adc_sum);
  }

  double total_neutrinos = tempADCInt_adcsum_nu_cos->Integral();
  double total_cosmics = tempADCInt_adcsum_cos->Integral();

  std::cout << "total_neutrinos = " << total_neutrinos << std::endl;
  std::cout << "total_cosmics = " << total_cosmics << std::endl;

  tempADCInt_adcsum_cos->Reset();
  tempADCInt_adcsum_nu_cos->Reset();
  v_numu_adc_sum.clear();
  v_nue_adc_sum.clear();
  v_cos_adc_sum.clear();
  v_numu_tp_multiplicity.clear();
  v_nue_tp_multiplicity.clear();
  v_cos_tp_multiplicity.clear();

  for (const auto &cut : ADCCuts) {
    std::vector<std::pair<int, bool>> numu_adc_int_cut_events, nue_adc_int_cut_events, cos_adc_int_cut_events;
    for (auto &event : numu_tpc_events.GetTPCEvents()) {
      if (event) {
        bool pass_adcint_cut(false);
        if (alg_type == 0) {
          pass_adcint_cut = cut::ADCIntegralSumCut(cut, drift_window, v_numu_adc_sum, v_numu_adc_sum_events_cut, v_numu_tp_multiplicity, event);
        } else if (alg_type == 1) {
          pass_adcint_cut = cut::TimeFilterAlg(cut, drift_window, v_numu_adc_sum, v_numu_adc_sum_events_cut, v_numu_tp_multiplicity, event);
        }
        int event_num = event->GetEventNum();
        numu_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
      }
    }
    for (auto &event : nue_tpc_events.GetTPCEvents()) {
      if (event) {
        bool pass_adcint_cut(false);
        if (alg_type == 0) {
          pass_adcint_cut = cut::ADCIntegralSumCut(cut, drift_window, v_nue_adc_sum, v_nue_adc_sum_events_cut, v_nue_tp_multiplicity, event);
        } else if (alg_type == 1) {
          pass_adcint_cut = cut::TimeFilterAlg(cut, drift_window, v_nue_adc_sum, v_nue_adc_sum_events_cut, v_nue_tp_multiplicity, event);
        }
        int event_num = event->GetEventNum();
        nue_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
      }
    }
    for (auto &event : cos_tpc_events.GetTPCEvents()) {
      if (event) {
        bool pass_adcint_cut(false);
        if (alg_type == 0) {
          pass_adcint_cut = cut::ADCIntegralSumCut(cut, drift_window, v_cos_adc_sum, v_cos_adc_sum_events_cut, v_cos_tp_multiplicity, event);
        } else if (alg_type == 1) {
          pass_adcint_cut = cut::TimeFilterAlg(cut, drift_window, v_cos_adc_sum, v_cos_adc_sum_events_cut, v_cos_tp_multiplicity, event);
        }
        int event_num = event->GetEventNum();
        cos_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
      }
    }

    int count1(0), count2(0), count3(0);
    for (const auto adc_sum : v_numu_adc_sum) {
      if (numu_adc_int_cut_events.at(count1).second) {
        tempADCInt_adcsum_nu_cos->Fill((double)adc_sum, weight_numu);
      }
      count1++;
    }
    for (const auto adc_sum : v_nue_adc_sum) {
      if (nue_adc_int_cut_events.at(count2).second) {
        tempADCInt_adcsum_nu_cos->Fill((double)adc_sum, weight_nue);
      }
      count2++;
    }
    for (const auto adc_sum : v_cos_adc_sum) {
      if (cos_adc_int_cut_events.at(count3).second) {
        tempADCInt_adcsum_cos->Fill((double)adc_sum);
      }
      count3++;
    }
    double eff_nu_adcintsum = 1.0 * tempADCInt_adcsum_nu_cos->Integral() / total_neutrinos;
    double eff_cos_adcintsum = 1.0 * tempADCInt_adcsum_cos->Integral() / total_cosmics;
    ADCCutEff_nu_cos.push_back(eff_nu_adcintsum);
    ADCCutEff_cos.push_back(eff_cos_adcintsum);
    
    tempADCInt_adcsum_cos->Reset();
    tempADCInt_adcsum_nu_cos->Reset();
    v_numu_adc_sum.clear();
    v_nue_adc_sum.clear();
    v_cos_adc_sum.clear();
    v_numu_tp_multiplicity.clear();
    v_nue_tp_multiplicity.clear();
    v_cos_tp_multiplicity.clear();
  }

  delete tempADCInt_adcsum_cos;
  delete tempADCInt_adcsum_nu_cos;
}

//------------------------------------
void ApplyADCIntegralThreshold(std::vector<int> &v_adc_sum, std::vector<int> &v_adc_sum_events_cut, std::vector<int> &v_tp_multiplicity, double &ADC_SUM_CUT, 
                               double &drift_window, std::vector<std::pair<int, bool>> &adc_int_cut_events, TPCEvents &obj_tpc_events) {
  for (auto &event : obj_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_adc_sum, v_adc_sum_events_cut, v_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }

  for (auto &cut : adc_int_cut_events) {
      if (!cut.second) {
        obj_tpc_events.CutEvent(cut.first);
      }
  }
}

//------------------------------------
void ApplyTimeDataFilter(std::vector<int> &v_time_max_adc_sum, std::vector<int> &v_time_max_adc_sum_events_cut, std::vector<int> &v_time_max_tp_multiplicity, 
                         double &time_filter_ADC_CUT, double &time_filter_window, std::vector<std::pair<int, bool>> &time_filter_events, TPCEvents &obj_tpc_events) {

  for (auto &event : obj_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_time_max_adc_sum, 
                                                 v_time_max_adc_sum_events_cut, v_time_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      time_filter_events.push_back({event_num, pass_time_filter});
    }
  }

  for (auto &cut : time_filter_events) {
      if (!cut.second) {
        obj_tpc_events.CutEvent(cut.first);
      }
  }
}

// Cut imlementations here. One day split these into their own file.

// Time filter for offline
//------------------------------------
bool cut::TimeFilterAlg(double const &adc_cut, double const &time_window, std::vector<int> &max_adc_sum, std::vector<int> &max_adc_sum_events_cut, std::vector<int> &max_tp_multiplicity, std::unique_ptr<Event> &tpc_event) {

  std::deque<TP> tp_collection;
  int event_max_adc_sum(0);
  int event_max_tp_multiplicity(0);
  for (const auto& tp : tpc_event->GetTPs()) {
    // Remove old TPs that are outside the time window
    while (!tp_collection.empty() && (tp.time_peak - tp_collection.front().time_peak > time_window)) {
      tp_collection.pop_front();
    }
    // Add current TP to the collection
    tp_collection.push_back(tp);
    // Calculate current ADC sum and TP multiplicity
    int current_adc_sum = 0;
    for (const auto& tp_in_window : tp_collection) {
      current_adc_sum += tp_in_window.adc_int;
    }
    int current_tp_multiplicity = tp_collection.size();

    // Update maximum values if current is greater
    if (current_adc_sum > event_max_adc_sum) {
      event_max_adc_sum = current_adc_sum;
    }
    if (current_tp_multiplicity > event_max_tp_multiplicity) {
      event_max_tp_multiplicity = current_tp_multiplicity;
    }
	}
	max_adc_sum.push_back(event_max_adc_sum);
	max_tp_multiplicity.push_back(event_max_tp_multiplicity);
	tp_collection.clear();

  if (event_max_adc_sum > adc_cut) {
    max_adc_sum_events_cut.push_back(event_max_adc_sum);
    return 1;
  }
  else return 0;
}

// ADC Intgral Sum cut for online
//------------------------------------
bool cut::ADCIntegralSumCut(double const &adc_cut, double const &time_window, std::vector<int> &adc_sum, std::vector<int> &adc_sum_events_cut, std::vector<int> &tp_multiplicity, std::unique_ptr<Event> &tpc_event) {

  int event_adc_sum(0);
  int event_tp_multiplicity(0);
  for (const auto& tp : tpc_event->GetTPs()) {
    // Add current TP to the collection
    event_adc_sum += tp.adc_int;
    event_tp_multiplicity++;
	}
	adc_sum.push_back(event_adc_sum);
	tp_multiplicity.push_back(event_tp_multiplicity);

  if (event_adc_sum > adc_cut) {
    adc_sum_events_cut.push_back(event_adc_sum);
    return 1;
  }
  else return 0;
}

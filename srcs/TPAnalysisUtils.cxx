#include "TPAnalysisUtils.h"
#include "NeutrinoEvent.h"
#include "CosmicEvent.h"

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
std::vector<std::unique_ptr<Event>> LoadTPFiles(std::string tpfile, int file_type) {

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
    v_TP.push_back(event_tp);
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


// Cut imlementations here. One day split these into their own file.

// Time filter for offline
//------------------------------------
bool cut::TimeFilterAlg(double const &adc_cut, double const &time_window, std::vector<int> &max_adc_sum, std::vector<int> &max_tp_multiplicity, std::unique_ptr<Event> &tpc_event) {

  std::deque<TP> tp_collection;
  int event_max_adc_sum(0);
  int event_max_tp_multiplicity(0);
  for (const auto& tp : tpc_event->GetTPs()) {
    // Remove old TPs that are outside the time window
    while (!tp_collection.empty() && (tp.time_peak - tp_collection.front().time_peak > time_window)) {
      tp_collection.pop_front();
    }
    if (!tp_collection.empty()) {
		  //std::cout << "Current time peak = " << tp.time_peak << ", earliest time peak = " << tp_collection.front().time_peak << ", WINDOW = " << tp.time_peak - tp_collection.front().time_peak << std::endl;
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

  if (event_max_adc_sum > adc_cut) return 1;
  else return 0;
}

// ADC Intgral Sum cut for online
//------------------------------------
bool cut::ADCIntegralSumCut(double const &adc_cut, double const &time_window, std::vector<int> &adc_sum, std::vector<int> &tp_multiplicity, std::unique_ptr<Event> &tpc_event) {

  int event_adc_sum(0);
  int event_tp_multiplicity(0);
  for (const auto& tp : tpc_event->GetTPs()) {
    // Add current TP to the collection
    event_adc_sum += tp.adc_int;
    event_tp_multiplicity++;
	}
	adc_sum.push_back(event_adc_sum);
	tp_multiplicity.push_back(event_tp_multiplicity);

  if (event_adc_sum > adc_cut) return 1;
  else return 0;
}

#include "TP.h"
#include "Neutrino.h"
#include "TPAnalysisUtils.h"
#include "TPCEvents.h"

#include <iostream>

#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>

int ParseArgs(int argc, const char** argv, std::vector<std::string> &files, std::vector<std::string> &nuana_files);

int main(int argc, char const* argv[]) {
  
  double weight_numu = 0.78;
  double weight_nue = 0.22;

  int neutrino_type(1), cosmic_type(0);

  std::vector<std::string> files;
  std::vector<std::string> nuana_files;

  int parseArgResult = ParseArgs(argc, argv, files, nuana_files);
  if(parseArgResult != 0){
    return 1;
  }

  std::cout << "Files parsed." << std::endl;

  for (const auto &file : nuana_files) {
    std::cout << "Nu ana file: " << file << std::endl;
  }
  for (const auto &file : files) {
    std::cout << "TP file: " << file << std::endl;
  }

  std::vector<Neutrino> numu_nulist, nue_nulist;
  numu_nulist = LoadNuAnaFiles(nuana_files.at(0));
  nue_nulist = LoadNuAnaFiles(nuana_files.at(1));

  std::cout << "Neutrino analysis files loaded." << std::endl;


  std::vector<std::unique_ptr<Event>> numu_event_tps, nue_event_tps, cos_event_tps;

  numu_event_tps = LoadTPFiles(files.at(0), neutrino_type); 
  nue_event_tps = LoadTPFiles(files.at(1), neutrino_type); 
  cos_event_tps = LoadTPFiles(files.at(2), cosmic_type); 

  std::cout << "TP files loaded." << std::endl;

  for (int i = 0; i < (int)numu_event_tps.size(); i++) {
    numu_event_tps.at(i)->InitialiseTruth(numu_nulist.at(i));
  }
  for (int i = 0; i < (int)nue_event_tps.size(); i++) {
    nue_event_tps.at(i)->InitialiseTruth(nue_nulist.at(i));
  }

  std::cout << "Overlay cosmic background." << std::endl;

  TRandom3 randomGenerator(0);
  for (int i = 0; i < (int)numu_event_tps.size(); i++) {
    // Choose a random cosmic overlay event
    int randomNumber_numu = randomGenerator.Integer(400);
    numu_event_tps.at(i)->AddEventTPs(cos_event_tps.at(randomNumber_numu)->GetTPs());
	} // end random number loop
  for (int i = 0; i < (int)nue_event_tps.size(); i++) {
    // Choose a random cosmic overlay event
    int randomNumber_nue = randomGenerator.Integer(400);
    nue_event_tps.at(i)->AddEventTPs(cos_event_tps.at(randomNumber_nue)->GetTPs());
	} // end random number loop

  std::cout << "Define TPC objects." << std::endl;

  // Create TPC objects witha list of events
  TPCEvents numu_tpc2_events(2, std::move(numu_event_tps), neutrino_type);
  TPCEvents numu_tpc5_events(5, std::move(numu_event_tps), neutrino_type);
  TPCEvents numu_tpc6_events(6, std::move(numu_event_tps), neutrino_type);

  TPCEvents nue_tpc2_events(2, std::move(nue_event_tps), neutrino_type);
  TPCEvents nue_tpc5_events(5, std::move(nue_event_tps), neutrino_type);
  TPCEvents nue_tpc6_events(6, std::move(nue_event_tps), neutrino_type);
  
  TPCEvents cos_tpc2_events(2, std::move(cos_event_tps), cosmic_type);
  TPCEvents cos_tpc5_events(5, std::move(cos_event_tps), cosmic_type);
  TPCEvents cos_tpc6_events(6, std::move(cos_event_tps), cosmic_type);

  // Remove events where neutrino interacts otside specific TPC
  numu_tpc2_events.CutOutOfTPCNeutrinos();
  numu_tpc5_events.CutOutOfTPCNeutrinos();
  numu_tpc6_events.CutOutOfTPCNeutrinos();
  
  nue_tpc2_events.CutOutOfTPCNeutrinos();
  nue_tpc5_events.CutOutOfTPCNeutrinos();
  nue_tpc6_events.CutOutOfTPCNeutrinos();

  // ADC Integral Sum Cut
  //----------------------------------------
  std::vector<int> v_numu_adc_sum, v_nue_adc_sum, v_cos_adc_sum;
  std::vector<int> v_numu_tp_multiplicity, v_nue_tp_multiplicity, v_cos_tp_multiplicity;
  std::vector<std::pair<int, bool>> numu_adc_int_cut_events, nue_adc_int_cut_events, cos_adc_int_cut_events;
  double ADC_SUM_CUT(10e6);
  double drift_window(150e3);
  for (auto &event : numu_tpc2_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_numu_adc_sum, v_numu_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      numu_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }
  for (auto &event : nue_tpc2_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_nue_adc_sum, v_nue_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      nue_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }
  for (auto &event : cos_tpc2_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_cos_adc_sum, v_cos_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      cos_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }

  for (auto &cut : numu_adc_int_cut_events) {
      if (!cut.second) {
        numu_tpc2_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : nue_adc_int_cut_events) {
      if (!cut.second) {
        nue_tpc2_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : cos_adc_int_cut_events) {
      if (!cut.second) {
        cos_tpc2_events.CutEvent(cut.first);
      }
  }

  // Time Filter
  //----------------------------------------
  std::vector<int> v_numu_max_adc_sum, v_nue_max_adc_sum, v_cos_max_adc_sum;
  std::vector<int> v_numu_max_tp_multiplicity, v_nue_max_tp_multiplicity, v_cos_max_tp_multiplicity;
  std::vector<std::pair<int, bool>> numu_time_filter_events, nue_time_filter_events, cos_time_filter_events;
  double time_filter_ADC_CUT(4e6);
  double time_filter_window(20e3);

  for (auto &event : numu_tpc2_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_numu_max_adc_sum, v_numu_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      numu_adc_int_cut_events.push_back({event_num, pass_time_filter});
    }
  }
  for (auto &event : nue_tpc2_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_nue_max_adc_sum, v_nue_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      nue_adc_int_cut_events.push_back({event_num, pass_time_filter});
    }
  }
  for (auto &event : cos_tpc2_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_cos_max_adc_sum, v_cos_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      cos_adc_int_cut_events.push_back({event_num, pass_time_filter});
    }
  }

  for (auto &cut : numu_time_filter_events) {
      if (!cut.second) {
        numu_tpc2_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : nue_time_filter_events) {
      if (!cut.second) {
        nue_tpc2_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : cos_time_filter_events) {
      if (!cut.second) {
        cos_tpc2_events.CutEvent(cut.first);
      }
  }
  
  // Cuts applied, now fill histograms
  //----------------------------------------
  std::cout << "Cuts applied, now fill histograms." << std::endl;
  
  TH1D *hADCInt_adcsum_cos = new TH1D("hADCIntSumCosmic", "ADC Integral Sum Distribution" , 50, 0, 25e6);
  hADCInt_adcsum_cos->SetDirectory(nullptr);
  TH1D *hADCInt_adcsum_nu_cos = new TH1D("hADCIntSumNeutrino", "ADC Integral Sum Distribution" , 50, 0, 25e6);
  hADCInt_adcsum_nu_cos->SetDirectory(nullptr);

  for (const auto adc_sum : v_numu_adc_sum) {
    hADCInt_adcsum_nu_cos->Fill((double)adc_sum, weight_numu);
  }
  for (const auto adc_sum : v_nue_adc_sum) {
    hADCInt_adcsum_nu_cos->Fill((double)adc_sum, weight_nue);
  }
  for (const auto adc_sum : v_cos_adc_sum) {
    hADCInt_adcsum_cos->Fill((double)adc_sum);
  }

  std::cout << "Histograms filled." << std::endl;

  hADCInt_adcsum_cos->Scale(1/hADCInt_adcsum_cos->Integral());
  hADCInt_adcsum_nu_cos->Scale(1/hADCInt_adcsum_nu_cos->Integral());

  hADCInt_adcsum_cos->SetLineWidth(3);
  hADCInt_adcsum_cos->SetLineColor(kBlue+1);
  hADCInt_adcsum_nu_cos->SetLineWidth(3);
  hADCInt_adcsum_nu_cos->SetLineColor(kRed+1);
  
  hADCInt_adcsum_cos->GetXaxis()->SetTitle("ADC Integral Sum (ADC)");
  hADCInt_adcsum_cos->GetXaxis()->SetTitleSize(0.045);
  hADCInt_adcsum_cos->GetXaxis()->SetLabelSize(0.045);
  hADCInt_adcsum_cos->GetYaxis()->SetTitleSize(0.045);
  hADCInt_adcsum_cos->GetYaxis()->SetLabelSize(0.045);
  
  hADCInt_adcsum_nu_cos->GetXaxis()->SetTitle("ADC Integral Sum (ADC)");
  hADCInt_adcsum_nu_cos->GetXaxis()->SetTitleSize(0.045);
  hADCInt_adcsum_nu_cos->GetXaxis()->SetLabelSize(0.045);
  hADCInt_adcsum_nu_cos->GetYaxis()->SetTitleSize(0.045);
  hADCInt_adcsum_nu_cos->GetYaxis()->SetLabelSize(0.045);

  TH1D *hADCInt_time_cos = new TH1D("hTimeFilterCosmic", "ADC Integral Sum Distribution in 20k Tick Window" , 50, 0, 25e6);
  hADCInt_time_cos->SetDirectory(nullptr);
  TH1D *hADCInt_time_nu_cos = new TH1D("hTimeFilterNeutrino", "ADC Integral Sum Distribution in 20k Tick Window" , 50, 0, 25e6);
  hADCInt_time_nu_cos->SetDirectory(nullptr);

  for (const auto max_adc_sum : v_numu_max_adc_sum) {
    hADCInt_time_nu_cos->Fill((double)max_adc_sum, weight_numu);
  }
  for (const auto max_adc_sum : v_nue_max_adc_sum) {
    hADCInt_time_nu_cos->Fill((double)max_adc_sum, weight_nue);
  }
  for (const auto max_adc_sum : v_cos_max_adc_sum) {
    hADCInt_time_cos->Fill((double)max_adc_sum);
  }

  std::cout << "Histograms filled." << std::endl;

  hADCInt_time_cos->Scale(1/hADCInt_time_cos->Integral());
  hADCInt_time_nu_cos->Scale(1/hADCInt_time_nu_cos->Integral());

  hADCInt_time_cos->SetLineWidth(3);
  hADCInt_time_cos->SetLineColor(kBlue+1);
  hADCInt_time_nu_cos->SetLineWidth(3);
  hADCInt_time_nu_cos->SetLineColor(kRed+1);
  
  hADCInt_time_cos->GetXaxis()->SetTitle("ADC Integral Sum (ADC)");
  hADCInt_time_cos->GetXaxis()->SetTitleSize(0.045);
  hADCInt_time_cos->GetXaxis()->SetLabelSize(0.045);
  hADCInt_time_cos->GetYaxis()->SetTitleSize(0.045);
  hADCInt_time_cos->GetYaxis()->SetLabelSize(0.045);
  
  hADCInt_time_nu_cos->GetXaxis()->SetTitle("ADC Integral Sum (ADC)");
  hADCInt_time_nu_cos->GetXaxis()->SetTitleSize(0.045);
  hADCInt_time_nu_cos->GetXaxis()->SetLabelSize(0.045);
  hADCInt_time_nu_cos->GetYaxis()->SetTitleSize(0.045);
  hADCInt_time_nu_cos->GetYaxis()->SetLabelSize(0.045);

  TFile *outfile = new TFile("outTimeFilter.root", "RECREATE");
  hADCInt_adcsum_nu_cos->Write();
  hADCInt_adcsum_cos->Write();
  hADCInt_time_nu_cos->Write();
  hADCInt_time_cos->Write();

  outfile->Close();

  delete hADCInt_adcsum_nu_cos;
  delete hADCInt_adcsum_cos;
  delete hADCInt_time_nu_cos;
  delete hADCInt_time_cos;

  return 0;
}

int ParseArgs(int argc, const char** argv, std::vector<std::string> &files, std::vector<std::string> &nuana_files) {

  int opt(1);
  while (opt < argc) {
    if (std::string(argv[opt]) == "--tpnf") { // two nu tp files
      opt++;
      files.push_back(std::string(argv[opt]));
      opt++;
      files.push_back(std::string(argv[opt]));
    } else if (std::string(argv[opt]) == "--tpcf") { // one cosmic
      opt++;
      files.push_back(std::string(argv[opt]));
    } else if (std::string(argv[opt]) == "--annf") {
      opt++;
      nuana_files.push_back(std::string(argv[opt]));
      opt++;
      nuana_files.push_back(std::string(argv[opt]));
    } else {
      std::cout << "[ERROR] Unknown option." << std::endl;
      return 1;
    }
    opt++;
  }

  if (files.empty() || nuana_files.empty()) {
    std::cout << "[ERROR] Files not loaded." << std::endl;
    return 1;
  }

  return 0;

}


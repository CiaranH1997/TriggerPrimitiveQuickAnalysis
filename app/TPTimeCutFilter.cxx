#include "TP.h"
#include "Neutrino.h"
#include "TPAnalysisUtils.h"
#include "TPCEvents.h"

#include <iostream>

#include <TFile.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TTree.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>

int ParseArgs(int argc, const char** argv, std::vector<std::string> &files, std::vector<std::string> &nuana_files, int &TPC, bool &update);

int main(int argc, char const* argv[]) {
  
  double weight_numu = 0.78;
  double weight_nue = 0.22;

  int neutrino_type(1), cosmic_type(0);

  std::vector<std::string> files;
  std::vector<std::string> nuana_files;
  int TPC;
  bool update_output(false);

  int parseArgResult = ParseArgs(argc, argv, files, nuana_files, TPC, update_output);
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
    int randomNumber_numu = randomGenerator.Integer((int)cos_event_tps.size());
    numu_event_tps.at(i)->AddEventTPs(cos_event_tps.at(randomNumber_numu)->GetTPs());
	} // end random number loop
  for (int i = 0; i < (int)nue_event_tps.size(); i++) {
    // Choose a random cosmic overlay event
    int randomNumber_nue = randomGenerator.Integer((int)cos_event_tps.size());
    nue_event_tps.at(i)->AddEventTPs(cos_event_tps.at(randomNumber_nue)->GetTPs());
	} // end random number loop

  std::cout << "Define TPC objects." << std::endl;

  // Create TPC objects witha list of events
  TPCEvents numu_tpc_events(TPC, std::move(numu_event_tps), neutrino_type);
  TPCEvents nue_tpc_events(TPC, std::move(nue_event_tps), neutrino_type);
  TPCEvents cos_tpc_events(TPC, std::move(cos_event_tps), cosmic_type);

  // Remove events where neutrino interacts otside specific TPC
  numu_tpc_events.CutOutOfTPCNeutrinos();
  nue_tpc_events.CutOutOfTPCNeutrinos();

  // ADC Integral Sum Cut Efficiency
  //----------------------------------------
  std::vector<double> ADCCuts = {1e6, 2e6, 3e6, 4e6, 5e6, 6e6, 7e6, 8e6, 9e6, 10e6, 11e6, 12e6, 13e6, 14e6, 15e6, 16e6, 18e6, 20e6, 22e6, 24e6};
  std::vector<double> ADCCutEff_nu_cos, ADCCutEff_cos;

  TH1D *tempADCInt_adcsum_cos = new TH1D("temp1", "ADC Integral Sum Distribution" , 50, 0, 25e6);
  tempADCInt_adcsum_cos->SetDirectory(nullptr);
  TH1D *tempADCInt_adcsum_nu_cos = new TH1D("temp2", "ADC Integral Sum Distribution" , 50, 0, 25e6);
  tempADCInt_adcsum_nu_cos->SetDirectory(nullptr);
  for (const auto &cut : ADCCuts) {
    double drift_window(150e3);
    std::vector<int> v_numu_adc_sum, v_nue_adc_sum, v_cos_adc_sum;
    std::vector<int> v_numu_tp_multiplicity, v_nue_tp_multiplicity, v_cos_tp_multiplicity;
    std::vector<std::pair<int, bool>> numu_adc_int_cut_events, nue_adc_int_cut_events, cos_adc_int_cut_events;
    for (auto &event : numu_tpc_events.GetTPCEvents()) {
      if (event) {
        bool pass_adcint_cut = cut::ADCIntegralSumCut(cut, drift_window, v_numu_adc_sum, v_numu_tp_multiplicity, event);
        int event_num = event->GetEventNum();
        numu_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
      }
    }
    for (auto &event : nue_tpc_events.GetTPCEvents()) {
      if (event) {
        bool pass_adcint_cut = cut::ADCIntegralSumCut(cut, drift_window, v_nue_adc_sum, v_nue_tp_multiplicity, event);
        int event_num = event->GetEventNum();
        nue_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
      }
    }
    for (auto &event : cos_tpc_events.GetTPCEvents()) {
      if (event) {
        bool pass_adcint_cut = cut::ADCIntegralSumCut(cut, drift_window, v_cos_adc_sum, v_cos_tp_multiplicity, event);
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

    double eff_nu_adcintsum = 1.0 * tempADCInt_adcsum_nu_cos->GetEntries() / (numu_tpc_events.GetTPCEvents().size() + nue_tpc_events.GetTPCEvents().size());
    double eff_cos_adcintsum = 1.0 * tempADCInt_adcsum_cos->GetEntries() / cos_tpc_events.GetTPCEvents().size();
    ADCCutEff_nu_cos.push_back(eff_nu_adcintsum);
    ADCCutEff_cos.push_back(eff_cos_adcintsum);
    tempADCInt_adcsum_cos->Reset();
    tempADCInt_adcsum_nu_cos->Reset();
  }

  delete tempADCInt_adcsum_cos;
  delete tempADCInt_adcsum_nu_cos;
	
  TGraph *gEffADCCut_cos = new TGraph((int)ADCCutEff_cos.size(), &ADCCuts[0], &ADCCutEff_cos[0]);
	TGraph *gEffADCCut_nu_cos = new TGraph((int)ADCCutEff_nu_cos.size(), &ADCCuts[0], &ADCCutEff_nu_cos[0]);
	
  std::string grtpc_str = "_tpc" + std::to_string(TPC);

  gEffADCCut_cos->SetName(("gADCIntSumCosEff" + grtpc_str).c_str());
  gEffADCCut_nu_cos->SetName(("gADCIntSumNuCosEff" + grtpc_str).c_str());

	gEffADCCut_cos->SetMarkerStyle(8);
	gEffADCCut_cos->SetLineWidth(4);
	gEffADCCut_cos->SetLineColor(kBlue+1);
	
	gEffADCCut_nu_cos->SetMarkerStyle(8);
	gEffADCCut_nu_cos->SetLineWidth(4);
	gEffADCCut_nu_cos->SetLineColor(kRed+1);

	TMultiGraph *mgeff = new TMultiGraph();
  mgeff->SetName(("mgADCIntSumNuCosEff" + grtpc_str).c_str());
	mgeff->Add(gEffADCCut_cos, "PL");
	mgeff->Add(gEffADCCut_nu_cos, "PL");
	
	mgeff->SetTitle(("ADC Integral Sum Cut Efficiency in TPC " +  std::to_string(TPC)).c_str());
	mgeff->GetXaxis()->SetTitle("ADC Integral Sum Cut (ADC)");
	mgeff->GetYaxis()->SetTitle("Efficiency (%)");
	mgeff->GetXaxis()->SetTitleSize(0.045);
	mgeff->GetXaxis()->SetLabelSize(0.045);
	mgeff->GetYaxis()->SetTitleSize(0.045);
	mgeff->GetYaxis()->SetLabelSize(0.045);


  // ADC Integral Sum Cut Optimal
  //----------------------------------------
  std::vector<int> v_numu_adc_sum, v_nue_adc_sum, v_cos_adc_sum;
  std::vector<int> v_numu_tp_multiplicity, v_nue_tp_multiplicity, v_cos_tp_multiplicity;
  std::vector<std::pair<int, bool>> numu_adc_int_cut_events, nue_adc_int_cut_events, cos_adc_int_cut_events;
  double ADC_SUM_CUT(10e6);
  double drift_window(150e3);
  for (auto &event : numu_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_numu_adc_sum, v_numu_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      numu_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }
  for (auto &event : nue_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_nue_adc_sum, v_nue_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      nue_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }
  for (auto &event : cos_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_adcint_cut = cut::ADCIntegralSumCut(ADC_SUM_CUT, drift_window, v_cos_adc_sum, v_cos_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      cos_adc_int_cut_events.push_back({event_num, pass_adcint_cut});
    }
  }

  for (auto &cut : numu_adc_int_cut_events) {
      if (!cut.second) {
        numu_tpc_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : nue_adc_int_cut_events) {
      if (!cut.second) {
        nue_tpc_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : cos_adc_int_cut_events) {
      if (!cut.second) {
        cos_tpc_events.CutEvent(cut.first);
      }
  }

  // Time Filter
  //----------------------------------------
  std::vector<int> v_numu_max_adc_sum, v_nue_max_adc_sum, v_cos_max_adc_sum;
  std::vector<int> v_numu_max_tp_multiplicity, v_nue_max_tp_multiplicity, v_cos_max_tp_multiplicity;
  std::vector<std::pair<int, bool>> numu_time_filter_events, nue_time_filter_events, cos_time_filter_events;
  double time_filter_ADC_CUT(4e6);
  double time_filter_window(20e3);

  for (auto &event : numu_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_numu_max_adc_sum, v_numu_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      numu_adc_int_cut_events.push_back({event_num, pass_time_filter});
    }
  }
  for (auto &event : nue_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_nue_max_adc_sum, v_nue_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      nue_adc_int_cut_events.push_back({event_num, pass_time_filter});
    }
  }
  for (auto &event : cos_tpc_events.GetTPCEvents()) {
    if (event) {
      bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_cos_max_adc_sum, v_cos_max_tp_multiplicity, event);
      int event_num = event->GetEventNum();
      cos_adc_int_cut_events.push_back({event_num, pass_time_filter});
    }
  }

  for (auto &cut : numu_time_filter_events) {
      if (!cut.second) {
        numu_tpc_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : nue_time_filter_events) {
      if (!cut.second) {
        nue_tpc_events.CutEvent(cut.first);
      }
  }
  for (auto &cut : cos_time_filter_events) {
      if (!cut.second) {
        cos_tpc_events.CutEvent(cut.first);
      }
  }
  
  // Cuts applied, now fill histograms
  //----------------------------------------
  std::cout << "Cuts applied, now fill histograms." << std::endl;
 
  std::string tpc_str = "_tpc" + std::to_string(TPC);

  TH1D *hADCInt_adcsum_cos = new TH1D(("hADCIntSumCosmic" + tpc_str).c_str(), ("ADC Integral Sum Distribution in TPC " + std::to_string(TPC)).c_str(), 50, 0, 25e6);
  hADCInt_adcsum_cos->SetDirectory(nullptr);
  TH1D *hADCInt_adcsum_nu_cos = new TH1D(("hADCIntSumNeutrino" + tpc_str).c_str(), ("ADC Integral Sum Distribution in TPC " + std::to_string(TPC)).c_str(), 50, 0, 25e6);
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

  TH1D *hADCInt_time_cos = new TH1D(("hTimeFilterCosmic" + tpc_str).c_str(), ("ADC Integral Sum Distribution in 20k Tick Window in TPC " + std::to_string(TPC)).c_str(), 50, 0, 25e6);
  hADCInt_time_cos->SetDirectory(nullptr);
  TH1D *hADCInt_time_nu_cos = new TH1D(("hTimeFilterNeutrino" + tpc_str).c_str(), ("ADC Integral Sum Distribution in 20k Tick Window in TPC " + std::to_string(TPC)).c_str() , 50, 0, 25e6);
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

  TFile *outfile = new TFile("outTimeFilter.root", update_output ? "UPDATE" : "RECREATE");
  mgeff->Write();
  gEffADCCut_nu_cos->Write();
  gEffADCCut_cos->Write();
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

int ParseArgs(int argc, const char** argv, std::vector<std::string> &files, std::vector<std::string> &nuana_files, int &TPC, bool &update) {

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
    } else if (std::string(argv[opt]) == "--tpc") {
      opt++;
      TPC = std::strtol(argv[opt], nullptr, 0);
    } else if (std::string(argv[opt]) == "--u") {
      opt++;
      update = true;
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


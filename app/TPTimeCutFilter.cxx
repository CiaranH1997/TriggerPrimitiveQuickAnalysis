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

int ParseArgs(int argc, const char** argv, std::vector<std::string> files, std::vector<std::string> &nuana_files);

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

  std::vector<Neutrino> numu_nulist, nue_nulist;
  numu_nulist = LoadNuAnaFiles(nuana_files.at(0));
  nue_nulist = LoadNuAnaFiles(nuana_files.at(1));


  std::vector<std::unique_ptr<Event>> numu_event_tps, nue_event_tps, cos_event_tps;

  numu_event_tps = LoadTPFiles(files.at(0), neutrino_type); 
  nue_event_tps = LoadTPFiles(files.at(1), neutrino_type); 
  cos_event_tps = LoadTPFiles(files.at(2), cosmic_type); 

  //for (auto &numu : numu_event_tps) {
  for (int i = 0; i < (int)numu_event_tps.size(); i++) {
    numu_event_tps.at(i)->InitialiseTruth(numu_nulist.at(i));
  }
  for (int i = 0; i < (int)nue_event_tps.size(); i++) {
    nue_event_tps.at(i)->InitialiseTruth(nue_nulist.at(i));
  }


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

  TPCEvents numu_tpc2_events(2, std::move(numu_event_tps), neutrino_type);
  TPCEvents numu_tpc5_events(5, std::move(numu_event_tps), neutrino_type);
  TPCEvents numu_tpc6_events(6, std::move(numu_event_tps), neutrino_type);

  TPCEvents nue_tpc2_events(2, std::move(nue_event_tps), neutrino_type);
  TPCEvents nue_tpc5_events(5, std::move(nue_event_tps), neutrino_type);
  TPCEvents nue_tpc6_events(6, std::move(nue_event_tps), neutrino_type);
  
  TPCEvents cos_tpc2_events(2, std::move(cos_event_tps), cosmic_type);
  TPCEvents cos_tpc5_events(5, std::move(cos_event_tps), cosmic_type);
  TPCEvents cos_tpc6_events(6, std::move(cos_event_tps), cosmic_type);


  std::vector<int> v_numu_max_adc_sum, v_nue_max_adc_sum, v_cos_max_adc_sum;
  std::vector<int> v_numu_max_tp_multiplicity, v_nue_max_tp_multiplicity, v_cos_max_tp_multiplicity;
  double time_filter_ADC_CUT(4e6);
  double time_filter_window(20e3);

  for (auto &event : numu_tpc2_events.GetTPCEvents()) {
    bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_numu_max_adc_sum, v_numu_max_tp_multiplicity, event);
    if (!pass_time_filter) numu_tpc2_events.CutEvent(event);
  }
  for (auto &event : nue_tpc2_events.GetTPCEvents()) {
    bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_nue_max_adc_sum, v_nue_max_tp_multiplicity, event);
    if (!pass_time_filter) nue_tpc2_events.CutEvent(event);
  }
  for (auto &event : cos_tpc2_events.GetTPCEvents()) {
    bool pass_time_filter = cut::TimeFilterAlg(time_filter_ADC_CUT, time_filter_window, v_cos_max_adc_sum, v_cos_max_tp_multiplicity, event);
    if (!pass_time_filter) cos_tpc2_events.CutEvent(event);
  }


  TH1D *hADCInt_time_cos = new TH1D("htcos", "ADC Integral Sum Distribution in 20k Tick Window" , 50, 0, 25E6);
  hADCInt_time_cos->SetDirectory(nullptr);
  TH1D *hADCInt_time_nu_cos = new TH1D("htnucos", "htnucos" , 50, 0, 25E6);
  hADCInt_time_nu_cos->SetDirectory(nullptr);

  for (const auto max_adc_sum : v_numu_max_adc_sum) {
    hADCInt_time_nu_cos->Fill(max_adc_sum, weight_numu);
  }
  for (const auto max_adc_sum : v_nue_max_adc_sum) {
    hADCInt_time_nu_cos->Fill(max_adc_sum, weight_nue);
  }
  for (const auto max_adc_sum : v_cos_max_adc_sum) {
    hADCInt_time_cos->Fill(max_adc_sum);
  }


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

  TLegend *hleg = new TLegend(0.5, 0.7, 0.8, 0.9);
  hleg->AddEntry(hADCInt_time_cos, "Cosmics");
  hleg->AddEntry(hADCInt_time_nu_cos, "#nu_{#mu} + #nu_{e} + Cosmics");

  gStyle->SetOptStat(0);
  new TCanvas;
  hADCInt_time_cos->Draw("HIST");
  hADCInt_time_nu_cos->Draw("HIST SAME");
  hleg->Draw();

  return 0;
}

int ParseArgs(int argc, const char** argv, std::vector<std::string> files, std::vector<std::string> &nuana_files) {

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


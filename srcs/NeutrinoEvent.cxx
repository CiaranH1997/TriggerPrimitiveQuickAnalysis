#include "NeutrinoEvent.h"

/*void NeutrinoEvent::AddEventTPs(std::vector<TP> TPs) {

  event_tps.insert(event_tps.end(), TPs.begin(), TPs.end());
  this->SortTPsByTime();
}*/


void NeutrinoEvent::InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) {

  if (truth_event == event_num) {
    nu = {truth_event, E, tpcid, pdg, ccnc};
  } else {
    std::cout << "[ERROR] nu and Event event ID misaligned!" << std::endl;
    exit(1);
  }

}

void NeutrinoEvent::InitialiseTruth(Neutrino in_nu) {

  if (event_num == in_nu.event_num) {
    nu = in_nu;
  } else {
    std::cout << "[ERROR] nu and Event event ID misaligned!" << std::endl;
    exit(1);
  }

}

Neutrino NeutrinoEvent::GetNeutrino() const {
  return nu;
}

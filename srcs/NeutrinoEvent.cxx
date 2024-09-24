#include "NeutrinoEvent.h"

void NeutrinoEvent::InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) {

  if (truth_event == event_num) {
    nu = {truth_event, E, tpcid, pdg, ccnc};
  } else {
    std::cout << "[ERROR] nu and Event event ID misaligned!" << std::endl;
    exit(1);
  }

}

void NeutrinoEvent::InitialiseTruth(Neutrino in_nu) {

  std::cout << "event_num = " << event_num << "; neutrino number = " << in_nu.event_num << std::endl;
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

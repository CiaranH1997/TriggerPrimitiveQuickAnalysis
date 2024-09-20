#include "NeutrinoEvent.h"

void NeutrinoEvent::AddEventTPs(std::vector<TP> TPs) {

  event_tps.insert(event_tps.end(), TPs.begin(), TPs.end());
  this->SortTPsByTime();
}


void NeutrinoEvent::InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) {

  if (truth_event == event_num) {
    nu_energy = E;
    tpc_id = tpcid;
    nu_pdg = pdg;
    nu_ccnc = ccnc;
  } else {
    std::cout << "[ERROR] nu and Event event ID misaligned!" << std::endl;
    exit(1);
  }

}

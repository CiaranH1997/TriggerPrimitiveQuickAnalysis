#include "CosmicEvent.h"

/*void CosmicEvent::AddEventTPs(std::vector<TP> TPs) {

  event_tps.insert(event_tps.end(), TPs.begin(), TPs.end());
  this->SortTPsByTime();
}*/

void CosmicEvent::InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) {

  std::cout << "[ERROR] Cosmics don't have truth!" << std::endl;
  exit(1);

}

void CosmicEvent::InitialiseTruth(Neutrino in_nu) {

  std::cout << "[ERROR] Cosmics don't have truth!" << std::endl;
  exit(1);

}

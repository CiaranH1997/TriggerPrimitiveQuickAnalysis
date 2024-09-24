#include "CosmicEvent.h"

void CosmicEvent::InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) {

  std::cout << "[ERROR] Cosmics don't have truth!" << std::endl;
  exit(1);

}

void CosmicEvent::InitialiseTruth(Neutrino in_nu) {

  std::cout << "[ERROR] Cosmics don't have truth!" << std::endl;
  exit(1);

}

#ifndef INC_COSEVENT_H_
#define INC_COSEVENT_H_

#include "Event.h"
#include "Neutrino.h"

#include <vector>

// A cosmic event contains the TPs in an event and its event number

class CosmicEvent : public Event {
  public:
    CosmicEvent(int ev_num, std::vector<TP> TPs) : Event(ev_num, TPs) {}
    
    void InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) override;
    void InitialiseTruth(Neutrino nu) override;
    Neutrino GetNeutrino() const override {
      std::cout << "[ERROR] Should not givng a cosmic event neutrino information." << std::endl;
      Neutrino nu;
      return nu;
    }
};

#endif

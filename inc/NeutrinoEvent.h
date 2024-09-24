#ifndef INC_NUEVENT_H_
#define INC_NUEVENT_H_

#include "Event.h"
#include "Neutrino.h"

#include <vector>

// An event contains the TPs in an event and its event number

class NeutrinoEvent : public Event {
  public:
    NeutrinoEvent(int ev_num, std::vector<TP> TPs) : Event(ev_num, TPs) {}
    NeutrinoEvent(int ev_num, std::vector<TP> TPs, Neutrino nu) : Event(ev_num, TPs) {
      this->InitialiseTruth(nu);
    }

    void InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) override;
    void InitialiseTruth(Neutrino nu) override;
    Neutrino GetNeutrino() const override;

  protected:
    Neutrino nu;
};

#endif

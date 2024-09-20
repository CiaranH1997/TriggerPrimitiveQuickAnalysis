#include "Event.h"
#include "TP.h"

#include <vector>

// An event contains the TPs in an event and its event number

class NeutrinoEvent : public Event {
  public:
    void AddEventTPs(std::vector<TP> TPs);

    void InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc);

  protected:
    //std::vector<TP> event_tps;
    //int event_num;
    double nu_energy;
    int tpc_id;
    int nu_pdg;
    int nu_ccnc;
};

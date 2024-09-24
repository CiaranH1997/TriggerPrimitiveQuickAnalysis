#ifndef INC_EVENT_H_
#define INC_EVENT_H_

#include "TP.h"
#include "Neutrino.h"

#include <vector>

// An event contains the TPs in an event and its event number

class Event {
  public:
    Event(int ev_num, std::vector<TP> TPs);

    virtual void AddEventTPs(std::vector<TP> TPs);

    virtual void SortTPsByTime();

    virtual std::vector<TP> GetTPs() const;

    virtual int GetEventNum() const;
    
    virtual void InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) = 0;
    virtual void InitialiseTruth(Neutrino nu) = 0;
    virtual Neutrino GetNeutrino() const = 0;

    virtual ~Event() {};


  protected:
    std::vector<TP> event_tps;
    int event_num;
};

#endif

#ifndef INC_TPCEVENTS_H_
#define INC_TPCEVENTS_H_

#include "NeutrinoEvent.h"
#include "CosmicEvent.h"

#include <vector>

// A class to hold TPs occuring in a specific TPC

class TPCEvents {
  public:
    TPCEvents(int tpc, std::vector<std::unique_ptr<Event>> events, int event_type);

    std::vector<std::unique_ptr<Event>> RemoveOutOfTPCHits(std::vector<std::unique_ptr<Event>> &events, int event_type);

    void CutOutOfTPCNeutrinos();

    int GetTPCID();

    std::vector<std::unique_ptr<Event>>& GetTPCEvents();

    void CutEvent(std::unique_ptr<Event> &event);
    void CutEvent(int &event_num);

  protected:
    int tpc_id;
    std::pair<int, int> TPC_chans;
    std::vector<std::unique_ptr<Event>> tpc_events;

  private:
    const std::pair<int, int> TPC1_chans = {2080, 2559};
    const std::pair<int, int> TPC5_chans = {7200, 7680};
    const std::pair<int, int> TPC2_chans = {4160, 4639};
    const std::pair<int, int> TPC6_chans = {9280, 9759}; 



};

#endif

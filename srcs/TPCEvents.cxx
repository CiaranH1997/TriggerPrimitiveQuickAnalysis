#include "TPCEvents.h"
#include "NeutrinoEvent.h"
#include "CosmicEvent.h"

TPCEvents::TPCEvents(int tpc, std::vector<std::unique_ptr<Event>> events, int event_type) {
  tpc_id = tpc;

  if (tpc_id == 1) {
    TPC_chans = TPC1_chans;
  } else if (tpc_id == 2) {
    TPC_chans = TPC2_chans;
  } else if (tpc_id == 5) {
    TPC_chans = TPC5_chans;
  } else if (tpc_id == 6) {
    TPC_chans = TPC6_chans;
  } else {
    std::cout << "[ERROR] Unknown TPC ID, must be 1, 2, 5, or 6" << std::endl;
    exit(1);
  }

  tpc_events = RemoveOutOfTPCHits(events, event_type);
      
}

std::vector<std::unique_ptr<Event>> TPCEvents::RemoveOutOfTPCHits(std::vector<std::unique_ptr<Event>> &in_events, int event_type) {
  std::vector<std::unique_ptr<Event>> ret;

  for (auto &ev : in_events) {
    std::vector<TP> tp_tpc;
    for (auto &tp : ev->GetTPs()) {
      if (tp.channel_id >= TPC_chans.first && tp.channel_id <= TPC_chans.second) {
        tp_tpc.push_back(tp);
      }
    }
    //std::unique_ptr<Event> ev_ret = std::make_unique<Event>(ev->GetEventNum(), tp_tpc);
    std::unique_ptr<Event> ev_ret;
    if (event_type == 0) {
      ev_ret = std::make_unique<CosmicEvent>(ev->GetEventNum(), tp_tpc);
    } else if (event_type == 1) {
      ev_ret = std::make_unique<NeutrinoEvent>(ev->GetEventNum(), tp_tpc);
    }
    ret.push_back(std::move(ev_ret));
  }

  return ret;
}

/*
void TPCEvents::CutOutOfTPCNeutrinos() {
  std::vector<std::unique_ptr<Event>> temp; 
  for (auto &evs : tpc_events) {
    if (evs->GetNeutrino().tpc_id == tpc_id) {
      temp.emplace_back(std::move(evs));
    }
  }
  tpc_events.clear();
  tpc_events = std::move(temp);
}
*/
int TPCEvents::GetTPCID() {
  return tpc_id;
}

std::vector<std::unique_ptr<Event>> TPCEvents::GetTPCEvents() {
  return std::move(tpc_events); 
}

//void TPCEvents::CutEvent(int event_num) {
void TPCEvents::CutEvent(std::unique_ptr<Event> &event) {
  std::vector<std::unique_ptr<Event>> temp;// = std::move(tpc_events);  
  for (int i = 0; i < (int)tpc_events.size(); i++) {
    // removing event_num from list of events in TPC
    //if (i+1 != event_num) temp.emplace_back(std::move(tpc_events.at(i)));
    if (i+1 != event->GetEventNum()) temp.emplace_back(std::move(tpc_events.at(i)));
  }
  tpc_events.clear();
  tpc_events = std::move(temp);
}

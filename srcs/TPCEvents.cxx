#include "TPCEvents.h"
#include "NeutrinoEvent.h"
#include "CosmicEvent.h"

//-------------------------------
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

//-------------------------------
std::vector<std::unique_ptr<Event>> TPCEvents::RemoveOutOfTPCHits(std::vector<std::unique_ptr<Event>> &in_events, int event_type) {
  std::vector<std::unique_ptr<Event>> ret;

  for (auto &ev : in_events) {
    std::vector<TP> tp_tpc;
    for (auto &tp : ev->GetTPs()) {
      if (tp.channel_id >= TPC_chans.first && tp.channel_id <= TPC_chans.second) {
        tp_tpc.push_back(tp);
      }
    }
    std::unique_ptr<Event> ev_ret;
    if (event_type == 0) {
      ev_ret = std::make_unique<CosmicEvent>(ev->GetEventNum(), tp_tpc);
    } else if (event_type == 1) {
      ev_ret = std::make_unique<NeutrinoEvent>(ev->GetEventNum(), tp_tpc, ev->GetNeutrino());
    }
    ret.push_back(std::move(ev_ret));
  }

  return ret;
}

//-------------------------------
void TPCEvents::CutOutOfTPCNeutrinos() {
  std::vector<std::unique_ptr<Event>> temp; 
  for (auto &evs : this->tpc_events) {
    if (evs) {
      std::cout << "GetNeutrino() tpc id = " << evs->GetNeutrino().tpc_id << "; tpc id in TPCEvents = " << tpc_id << std::endl;
      if (evs->GetNeutrino().tpc_id == tpc_id) {
        std::cout << "Neutrino in TPC " << tpc_id << std::endl;
        temp.emplace_back(std::move(evs));
      }
    }
  }
  std::cout << "Size of temp = " << temp.size() << "; And size of tpc_events = " << tpc_events.size() << std::endl;
  tpc_events.clear();
  tpc_events = std::move(temp);
  std::cout << "Size of tpc_events NOW = " << tpc_events.size() << std::endl;
}

//-------------------------------
int TPCEvents::GetTPCID() {
  return tpc_id;
}

//-------------------------------
std::vector<std::unique_ptr<Event>>& TPCEvents::GetTPCEvents() {
  return tpc_events; 
}

//-------------------------------
void TPCEvents::CutEvent(std::unique_ptr<Event> &event) {
  std::vector<std::unique_ptr<Event>> temp; 
  for (auto &evs : tpc_events) {
    if (evs && evs->GetEventNum() != event->GetEventNum()) {
      temp.emplace_back(std::move(evs));
    } else if (evs) { 
      std::cout << "Removing event " << event->GetEventNum() << std::endl; 
    } else {
      std::cout << "[WARNING] Null event pointer." << std::endl;
    }
  }
  tpc_events.clear();
  tpc_events = std::move(temp);
}

//-------------------------------
void TPCEvents::CutEvent(int &event_num) {
  std::vector<std::unique_ptr<Event>> temp; 
  for (auto &evs : tpc_events) {
    if (evs && evs->GetEventNum() != event_num) {
      temp.emplace_back(std::move(evs));
    } else if (evs) { 
      std::cout << "Removing event " << event_num << std::endl; 
    } else {
      std::cout << "[WARNING] Null event pointer." << std::endl;
    }
  }
  tpc_events.clear();
  tpc_events = std::move(temp);
}

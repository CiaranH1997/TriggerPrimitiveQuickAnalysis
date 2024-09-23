#include "Event.h"

Event::Event(int ev_num, std::vector<TP> TPs) {
  event_num = ev_num;
  for (const auto &tp : TPs) {
    // Only have TPs from collection plane
    if (tp.rop_id == 2 || tp.rop_id == 3) {
      event_tps.emplace_back(tp);
    }
  }
}

void Event::AddEventTPs(std::vector<TP> TPs) {
//void Event::AddEventTPs(std::unique_ptr<Event> TPs) {

  event_tps.insert(event_tps.end(), TPs.begin(), TPs.end());
  this->SortTPsByTime();
}

void Event::SortTPsByTime() {
  std::sort(event_tps.begin(), event_tps.end());
}

std::vector<TP> Event::GetTPs() const {
  return event_tps;
}

int Event::GetEventNum() const {
  return event_num;
}
    
/*void Event::InitialiseTruth(int truth_event, double E, int tpcid, int pdg, int ccnc) {
  std::cout << "[ERROR] Should only initialise truth of a specific particle type." << std::endl;
  exit(1);
}
void Event::InitialiseTruth(Neutrino nu) { // Should never be here 
  std::cout << "[ERROR] Should only initialise truth of a specific particle type." << std::endl;
  exit(1);
}*/

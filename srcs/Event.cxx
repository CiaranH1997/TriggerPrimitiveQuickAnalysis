#include "Event.h"

Event::Event(int ev_num, std::vector<TP> TPs) {
  event_num = ev_num;
  for (const auto &tp : TPs) {
    // Only have TPs from collection plane
    if (tp.rop_id == 2 || tp.rop_id == 3) {
      event_tps.emplace_back(tp);
    }
  }
  this->SortTPsByTime();
}

void Event::AddEventTPs(std::vector<TP> TPs) {
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
    

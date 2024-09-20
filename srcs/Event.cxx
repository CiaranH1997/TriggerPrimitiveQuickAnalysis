#include "Event.h"

Event::Event(int ev_num, std::vector<TP> TPs) {
  event_tps = TPs;
  event_num = ev_num;
}

void Event::SortTPsByTime() {
  std::sort(event_tps.begin(), event_tps.end());
}

std::vector<TP> Event::GetTPs() const {
  return event_tps;
}

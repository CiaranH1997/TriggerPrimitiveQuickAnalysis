#include "NeutrinoEvent.h"

void NeutrinoEvent::AddEventTPs(std::vector<TP> TPs) {

  event_tps.insert(event_tps.end(), TPs.begin(), TPs.end());
  this->SortTPsByTime();
}


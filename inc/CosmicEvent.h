#include "Event.h"
#include "TP.h"

#include <vector>

// A cosmic event contains the TPs in an event and its event number

class CosmicEvent : public Event {
  public:
    void AddEventTPs(std::vector<TP> TPs);

  protected:
    //std::vector<TP> event_tps;
    //int event_num;
};

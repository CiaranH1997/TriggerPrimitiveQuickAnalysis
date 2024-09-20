#include "TP.h"

#include <vector>

// An event contains the TPs in an event and its event number

class Event {
  public:
    Event(int ev_num, std::vector<TP> TPs);

    virtual void AddEventTPs(std::vector<TP> TPs) = 0;

    virtual void SortTPsByTime();

    virtual std::vector<TP> GetTPs() const;

    virtual ~Event() {};


  protected:
    std::vector<TP> event_tps;
    int event_num;
};

#ifndef INC_TP_H_
#define INC_TP_H_

#include <iostream>

struct TP {
  unsigned int channel_id;
  int rop_id;
  long time_peak;
  unsigned int adc_int;
  int event;
  // Add a comparison operator for sorting that is marked as const
  bool operator<(const TP& other) const {
    return time_peak < other.time_peak;
  }
};

#endif /* INC_TP_H_ */

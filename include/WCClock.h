#ifndef WC_CLOCK_H
#define WC_CLOCK_H

#include <stdlib.h>
#include <time.h>

#include "WCErrorHandler.h"

struct WCClock;
typedef struct WCClock WCClock;

WCClock * wc_clock_create(WCError * error);
void wc_clock_destroy(WCClock * wc_clock, WCError * error);

void wc_clock_start(WCClock * wc_clock, WCError * error);
void wc_clock_stop(WCClock * wc_clock, WCError * error);

double wc_clock_get_interval(WCClock * wc_clock, WCError * error);

#endif

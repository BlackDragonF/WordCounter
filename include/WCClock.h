#ifndef WC_CLOCK_H
#define WC_CLOCK_H

/* include necessary libraries */
#include <stdlib.h>
#include <time.h>

/* include error module */
#include "WCErrorHandler.h"

/* struct declaration */
struct WCClock;
typedef struct WCClock WCClock;

/* function list */

/* construct and destruct */
WCClock * wc_clock_create(WCError * error);
void wc_clock_destroy(WCClock * wc_clock, WCError * error);

/* clock operations */
void wc_clock_start(WCClock * wc_clock, WCError * error);
void wc_clock_stop(WCClock * wc_clock, WCError * error);

/* get operations */
double wc_clock_get_interval(WCClock * wc_clock, WCError * error);

#endif

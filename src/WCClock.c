#include "WCClock.h"

/* internal enum definition */
typedef enum WCClockState {WCClockRun, WCClockStop} WCClockState;

/* struct definition */
struct WCClock {
    WCClockState state;
    clock_t clock;
    double interval;
};

/* construct a WCClock struct */
WCClock * wc_clock_create(WCError * error) {
    WCClock * wc_clock = malloc(sizeof(WCClock));
    if (wc_clock == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    wc_clock->state = WCClockStop;
    wc_clock->interval = 0;
    *error = WCNoneError;
    return wc_clock;
}

/* destruct given WCClock struct */
void wc_clock_destroy(WCClock * wc_clock, WCError * error) {
    if (wc_clock == NULL) {
        *error = WCNullPointerError;
        return;
    }
    free(wc_clock);
    *error = WCNoneError;
}

/* start timing, failed if the clock has started */
void wc_clock_start(WCClock * wc_clock, WCError * error) {
    if (wc_clock == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (wc_clock->state == WCClockRun) {
        *error = WCClockStateError;
        return;
    }
    wc_clock->clock = clock();
    wc_clock->state = WCClockRun;
    *error = WCNoneError;
}

/* stop timing, failed if the clock has stopped */
void wc_clock_stop(WCClock * wc_clock, WCError * error) {
    if (wc_clock == NULL) {
        *error = WCNullPointerError;
        return;
    }
    if (wc_clock->state == WCClockStop) {
        *error = WCClockStateError;
        return;
    }
    clock_t temp = clock();
    wc_clock->interval = (double)(temp - wc_clock->clock)/CLOCKS_PER_SEC;
    wc_clock->clock = temp;
    wc_clock->state = WCClockStop;
    *error = WCNoneError;
}

/* get the time interval between last start and stop */
double wc_clock_get_interval(WCClock * wc_clock, WCError * error) {
    if (wc_clock == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    if (wc_clock->state == WCClockRun) {
        *error = WCClockStateError;
        return 0;
    }
    *error = WCNoneError;
    return wc_clock->interval;
}

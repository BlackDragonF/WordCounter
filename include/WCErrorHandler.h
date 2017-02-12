#ifndef WC_ERROR_HANDLER_H
#define WC_ERROR_HANDLER_H

typedef enum WCError {
    WCNoneError = 0,
    WCNullPointerError = 10,
    WCMemoryOverflowError = 11,

    WCIndexRangeError = 20,

    WCFileOpenError = 30,
    WCFileCloseError = 31,
    WCFileInternalError = 32,
    
    WCClockStateError = 40,
}WCError;

#endif

#ifndef WC_ERROR_HANDLER_H
#define WC_ERROR_HANDLER_H

typedef enum WCError {
    WCNoneError = 0,
    WCNullPointerError = 100,
    WCMemoryOverflowError,

    WCIndexRangeError = 200,

    WCFileOpenError = 300,
    WCFileCloseError,
    WCFileInternalError,
}WCError;



#endif

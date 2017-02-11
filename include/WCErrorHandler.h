#ifndef WC_ERROR_HANDLER_H
#define WC_ERROR_HANDLER_H

typedef enum WCError {
    WCNoneError = 0,
    WCNullPointerError = 100,
    WCMemoryOverflowError = 101,

    WCIndexRangeError = 200,

    WCFileOpenError = 300,
    WCFileCloseError = 301,
    WCFileInternalError = 302,
    
}WCError;

#endif

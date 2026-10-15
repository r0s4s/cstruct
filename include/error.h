/* ./cstruct/include/error.h */

#ifndef CSTRUCT_ERROR_H
#define CSTRUCT_ERROR_H

typedef enum {
    CSTRUCT_SUCCESS         =  0,   // Success code
    CSTRUCT_ERR_INVALID_ARG = -1,   // Invalid parameter value
    CSTRUCT_ERR_OVERFLOW    = -2,   // Overflow error
    CSTRUCT_ERR_ALLOC       = -3,   // Allocation failure
    CSTRUCT_ERR_UNDERFLOW   = -4    // Operation on empty container
} ErrorCode;

#endif /* CSTRUCT_ERROR_H */

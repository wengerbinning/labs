#include "retcode.h"
#include "test.h"

__GLOBAL_STA_MSG_INIT(code0, 10) = {
    ERR_MSG_SET(0, "global state 0"),
    ERR_MSG_SET(1, "global state 1"),
    ERR_MSG_SET(2, "global state 2"),
    ERR_MSG_SET(5, "global state 5"),
    ERR_MSG_SET(8, "global state 8"),
};

__GLOBAL_ERR_MSG_INIT(code0, 10) = {
    ERR_MSG_SET(0, "global error 0"),
    ERR_MSG_SET(1, "global error 1"),
    ERR_MSG_SET(2, "global error 2"),
    ERR_MSG_SET(4, "global error 4"),
};


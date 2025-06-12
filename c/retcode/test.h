#ifndef __TEST_H__
#define __TEST_H__

#include "retcode.h"

GLOBAL_ERR_MSG_INIT(code0, 10);
GLOBAL_STA_MSG_INIT(code0, 10);

STATIC_STA_MSG_INIT(code1) = {
    STA_MSG_SET(0, "static state 0"),
    STA_MSG_SET(1, "static state 1"),
    STA_MSG_SET(2, "static state 2"),
};
STATIC_ERR_MSG_INIT(code1) = {
    ERR_MSG_SET(0, "static error 0"),
    ERR_MSG_SET(1, "static error 1"),
    ERR_MSG_SET(2, "static error 2"),
};

#endif /* __TEST_H__ */
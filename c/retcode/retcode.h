#ifndef __RETURN_CODE_H__
#define __RETURN_CODE_H__

#define ABS(code)   ((code < 0) ? (-code) : code)

/* State Return Code */
#define DEFAULT_STAMSG "Unknown state"
#define STA_MSG(name)   _STA_MSG(name)
#define _STA_MSG(name)  (stamsg_ ## name)
#define __STA_SET_MSG(name, code, msg)  STA_MSG(name)[code] = msg
#define __STA_GET_MSG(name, code) (STA_MSG(name)[code]) ? (STA_MSG(name)[code]) : DEFAULT_STAMSG
#define STA_MSG_SIZE(name)  (sizeof(STA_MSG(name))/sizeof(char *))
#define _STA_SET_MSG(name, code, msg) (ABS(code) < STA_MSG_SIZE(name) ? __STA_SET_MSG(name, ABS(code), msg))
#define _STA_GET_MSG(name, code) (ABS(code) < STA_MSG_SIZE(name) ? __STA_GET_MSG(name, ABS(code)) : DEFAULT_STAMSG)

#define STATIC_STA_MSG_INIT(name) static const char *_STA_MSG(name)[]
#define __GLOBAL_STA_MSG_INIT(name, cnt) const char *STA_MSG(name)[cnt]
#define GLOBAL_STA_MSG_INIT(name, cnt) extern __GLOBAL_STA_MSG_INIT(name, cnt)
#define STA_MSG_SET(code, msg) [code] = msg

/* Error Return Code */
#define DEFAULT_ERRMSG "Unknown error"
#define ERR_MSG(name)   _ERR_MSG(name)
#define _ERR_MSG(name)  (errmsg_ ## name)

#define __ERR_SET_MSG(name, code, msg) ERR_MSG(name)[code] = msg
#define __ERR_GET_MSG(name, code) (ERR_MSG(name)[code]) ? (ERR_MSG(name)[code]) : DEFAULT_ERRMSG
#define ERR_MSG_SIZE(name)  (sizeof(ERR_MSG(name))/sizeof(char *))
#define _ERR_SET_MSG(name, code, msg) (ABS(code) < ERR_MSG_SIZE(name) ? __ERR_SET_MSG(name, ABS(code), msg))
#define _ERR_GET_MSG(name, code) (ABS(code) < ERR_MSG_SIZE(name) ? __ERR_GET_MSG(name, ABS(code)) : DEFAULT_ERRMSG)

#define STATIC_ERR_MSG_INIT(name) static const char *_ERR_MSG(name)[]
#define __GLOBAL_ERR_MSG_INIT(name, cnt) const char *ERR_MSG(name)[cnt]
#define GLOBAL_ERR_MSG_INIT(name, cnt) extern __GLOBAL_ERR_MSG_INIT(name, cnt)
#define ERR_MSG_SET(code, msg) [code] = msg

/* Return Code */
#define _RET_SET_MSG(name, code, msg) \
    ((code < 0) ? _ERR_SET_MSG(name, code, msg) : _STA_SET_MSG(name, code, msg))
#define _RET_GET_MSG(name, code) \
    ((code < 0) ? _ERR_GET_MSG(name, code) : _STA_GET_MSG(name, code))




/*

STATIC_STA_MSG_INIT(demo0) = {
    STA_MSG_SET(0, "state 0"),
    STA_MSG_SET(1, "state 1"),
};





*/




#endif /* __RETURN_CODE_H__ */
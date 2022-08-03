#ifndef __RETMSG_H__
#define __RETMSG_H__

/* Return Message APIs

 * __RET_SET_MSG(name, ret, msg), RET_SET_MSG(ret, msg) -
 * __RET_GET_MSG(name, ret), RET_GET_MSG(ret) -

 Return Normal Message (default: DEFAULT_NORMSG)
 -----------------------------------------------
 * NORCODE(idx) -
 * NORSAFE(name, idx) -
 * _NOR_MSG(name), NOR_MSG(name) -
 * __NOR_SET_MSG(name, idx, msg) -
 * __NOR_GET_MSG(name, idx) -

 * NOR_MSG_SIZE(name) -
 * __STAT_NOR_MSG_INIT(name, size), STATIC_NOR_MSG_INIT(name, size) -
 * __GLOB_NOR_MSG_INIT(name, size), GLOBAL_NOR_MSG_INIT(name, size) -
 * NOR_MSG_SET(idx,msg) -

 * _NOR_SET_MSG(name, idx, msg), NOR_SET_MSG(, msg) -
 * _NOR_GET_MSG(name, idx), NOR_GET_MSG(idx) -

 Return Error Message (default: DEFAULT_ERRMSG)
 ----------------------------------------------
 * ERRCODE(idx) -
 * ERRSAFE(name, idx) -
 * _ERR_MSG(name), ERR_MSG(name) -
 * __ERR_GET_MSG(name, val, msg) -
 * __ERR_SET_MSG(name, val) -

 * ERR_MSG_SIZE(name) -
 * __STAT_ERR_MSG_INIT(name, size), STATIC_ERR_MSG_INIT(name, size) -
 * __GLOB_ERR_MSG_INIT(name, size), GLOBAL_ERR_MSG_INIT(name, size) -
 * ERR_MSG_SET(val, msg) -

 * _ERR_SET_MSG(name, val, msg), ERR_SET_MSG(val, msg) -
 * _ERR_GET_MSG(name, val), ERR_GET_MSG(val) -
 */
#define ABS(ret) ((ret < 0) ? (-ret) : ret)
#define MSG(msg, str) ((msg) ? (msg) : (str))
/* Return Normal Message APIs */
#define NORCODE(idx)   ( idx)
#define DEFAULT_NORMSG "Unknown status"
#define  NOR_MSG(name) _NOR_MSG(name)
#define _NOR_MSG(name) (normsg_ ## name)
#define __NOR_SET_MSG(name, idx, msg) NOR_MSG(name)[idx] = msg
#define __NOR_GET_MSG(name, idx) MSG((NOR_MSG(name)[idx]), DEFAULT_NORMSG)
#define NOR_MSG_SIZE(name) (sizeof(NOR_MSG(name))/sizeof(char *))
#define __STAT_NOR_MSG_INIT(name, size) const char *_NOR_MSG(name)[size]
#define STATIC_NOR_MSG_INIT(name, size) static __STAT_NOR_MSG_INIT (name, size)
#define __GLOB_NOR_MSG_INIT(name, size) const char *NOR_MSG(name)[size]
#define GLOBAL_NOR_MSG_INIT(name, size) extern __GLOB_NOR_MSG_INIT (name, size)
#define NOR_MSG_SET(idx, msg) [idx] = msg
#define NORSAFE(name, idx) (ABS(idx) < NOR_MSG_SIZE(name))
#define _NOR_SET_MSG(name, idx, msg) \
	((NORSAFE(name, idx)) ? (__NOR_SET_MSG(name, ABS(idx), msg)) : NULL)
#define _NOR_GET_MSG(name, idx) \
	((NORSAFE(name, idx)) ? (__NOR_GET_MSG(name, ABS(idx))) : DEFAULT_NORMSG)
/* Return Error Message APIs */
#define ERRCODE(idx)   (-idx)
#define DEFAULT_ERRMSG "Unknown error"
#define ERR_MSG(name) _ERR_MSG(name)
#define _ERR_MSG(name) (errmsg_ ## name)
#define __ERR_SET_MSG(name, idx, msg) ERR_MSG(name)[idx] = msg
#define __ERR_GET_MSG(name, idx) MSG((ERR_MSG(name)[idx]), DEFAULT_ERRMSG)
#define ERR_MSG_SIZE(name)  (sizeof(ERR_MSG(name))/sizeof(char *))
#define __STAT_ERR_MSG_INIT(name, size) const char *_ERR_MSG(name)[size]
#define STATIC_ERR_MSG_INIT(name, size) static __STAT_ERR_MSG_INIT (name, size)
#define __GLOB_ERR_MSG_INIT(name, size) const char *ERR_MSG(name)[size]
#define GLOBAL_ERR_MSG_INIT(name, size) extern __GLOB_ERR_MSG_INIT (name, size)
#define ERR_MSG_SET(idx, msg)  [idx] = msg
#define ERRSAFE(name, idx) (ABS(idx) < ERR_MSG_SIZE(name))
#define _ERR_SET_MSG(name, idx, msg) \
	(ERRSAFE(name, idx) ? (__ERR_SET_MSG(name, ABS(idx), msg)))
#define _ERR_GET_MSG(name, idx) \
	(ERRSAFE(name, idx) ? (__ERR_GET_MSG(name, ABS(idx))) : DEFAULT_ERRMSG)
/* Return Message APIs */
#define __RET_SET_MSG(name, ret, msg) \
	((ret < 0) ? _ERR_SET_MSG(name, ret, msg) : _NOR_SET_MSG(name, ret, msg))
#define __RET_GET_MSG(name, ret) \
	((ret < 0) ? _ERR_GET_MSG(name, ret) : _NOR_GET_MSG(name, ret))
/* #### APIs END #### */

#endif /* __RETMSG_H__ */

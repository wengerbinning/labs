#ifndef __CONTENT_TYPE_H__
#define __CONTENT_TYPE_H__

int content_type_parse (char *buf, char **_charset, char **_boundary);

#endif /* __CONTENT_TYPE_H__ */
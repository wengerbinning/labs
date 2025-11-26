#ifndef __MATHS_H__
#define __MATHS_H__

#define FLOOR(m, n) (((m)/(n)) * (n))
#define CEIL0(m, n) (FLOOR(m, n) + (((m)%(n)) ? (n) : 0))
#define CEIL1(m, n) ((((m) + (n) - 1)/(n)) * (n))


#define floor(m, n) (((m)/(n)) * (n))
#define ceil(m, n)  ((((m) + (n) - 1)/(n)) * (n))

# endif /* __MATHS_H__ */

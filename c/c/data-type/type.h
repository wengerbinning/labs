
#define STR(s) _STR(s)
#define _STR(s) #s

#define TYPE_NUMFMT(b, s, t) \
    snprintf((char *)b, s, "%s %d %%0%dX %%0%dX\n", STR(t), sizeof(t), (2 * sizeof(t)), (2 * sizeof(t)))
#define TYPE_FORMAT(b, s, t) \
    snprintf((char *)b, s, "%s %d\n",  STR(t), sizeof(t))

#define NUM_MIN(t) ((unsigned t)((t)0 - 1) >> 0)
#define NUM_MAX(t) ((unsigned t)((t)0 - 1) >> 1)


#define TYPE_PRINT(t) printf("%s -> %d, \t%s * -> %d\n",  STR(t), sizeof(t), STR(t), sizeof(t*))
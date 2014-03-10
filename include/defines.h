
#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifndef ROUND_UP
#define ROUND_UP(size, align) (((size) + ((align) - 1)) & ~((align) - 1))
#endif

#ifndef ROUND_DOWN
#define ROUND_DOWN(size, align)	((size) & ~((align) - 1))
#endif

#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x) ({				\
		int __x = (x);			\
		(__x < 0) ? -__x : __x;		\
            })
#endif

#ifndef NELEMENTS
#define NELEMENTS(a)  (sizeof(a) / sizeof(a[0]))
#endif

#ifndef OFFSET
#define OFFSET(type, member)  (int)&((type*)0)->member
#endif

#ifndef PTR_ADD
#define PTR_ADD(ptr, size)   (void *)((char *)(ptr) + (size))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array)   (sizeof(array) / sizeof(array[0]))
#endif


#define HIWORD(_v)		(((_v) >> 16) & 0x0000ffff)
#define LOWORD(_v)		((_v) & 0x0000ffff)

#endif


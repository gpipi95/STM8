#ifndef _UTILITY_H
#define _UTILITY_H

#define CAST_UC(a) (unsigned char)(a) /* cast to unsigned char */

#define SET_BIT8(a, b) a |= CAST_UC(0x01 << b)
#define CLEAR_BIT8(a, b) a &= CAST_UC(~(0x01 << b))

#endif /* ifndef _UTILITY_H */

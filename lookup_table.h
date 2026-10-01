#ifndef LOOKUP_TABLE_H
#define	LOOKUP_TABLE_H

#include "stdint.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define ROWS 64u
#define COLS 211u
    
extern const int16_t dE_lookup[ROWS][COLS];

#ifdef	__cplusplus
}
#endif

#endif	/* LOOKUP_TABLE_H */


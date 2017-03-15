#ifndef _AllFileHashValue_H_
#define _AllFileHashValue_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DEFAULT_HASH_NUMBER 20
#define FILL_DIVISOR 0.7

typedef struct AllFileHashValue {

	char 	**allFileHashValue;
	int		length;
	int		listSize;

}AllFileHashValue, *AFHV;

AFHV	getAllFileHashValue();

int		addHashValue(AFHV afhv, char *hashValueString);
char*	getHashValue(AFHV afhv, int index);

int		deleteAllFileHashValue(AFHV afhv);

#endif // !_AllFileHashValue_H_

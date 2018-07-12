#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	struct stat s;
	PMEMoid root;
	PMEMobjpool *pop;
	struct vector *vectorp;

	if (argc < 5) {
		printf("Usage: %s <pmem file> <3 numbers>\n", argv[0]);
		return 1;
	}

	if (stat(argv[1], &s) < 0) {
		pop = pmemobj_create(argv[1], LAYOUT_NAME, PMEMOBJ_MIN_POOL, 0666);
		if (pop == NULL) {
			perror("pmemobj_create");
			return 2;
		}
	} else {
		pop = pmemobj_open(argv[1], LAYOUT_NAME);
		if (pop == NULL) {
			perror("pmemobj_create");
			return 3;
		}
	}

	root = pmemobj_root(pop, sizeof (struct vector));
	vectorp = pmemobj_direct(root);
	TX_BEGIN(pop) {
		pmemobj_tx_add_range(root, 0, sizeof(struct vector));
		vectorp->x = atoi(argv[2]);
		vectorp->y = atoi(argv[3]);
		vectorp->z = atoi(argv[4]);
	} TX_END
	pmemobj_close(pop);
	return 0;
}

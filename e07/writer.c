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
	int p, n, i;
	struct stat s;
	PMEMobjpool *pop;
	TOID(struct my_root) root;
	TOID(struct rectangle) rect;

	if (argc < 3) {
		printf("Usage: %s <pmem file> <#objects>\n", argv[0]);
		return 1;
	}

	if (stat(argv[1], &s) < 0) {
		pop = pmemobj_create(argv[1], POBJ_LAYOUT_NAME(rect_calc), PMEMOBJ_MIN_POOL, 0666);
		if (pop == NULL) {
			perror("pmemobj_create");
			return 2;
		}
	} else {
		pop = pmemobj_open(argv[1], POBJ_LAYOUT_NAME(rect_calc));
		if (pop == NULL) {
			perror("pmemobj_create");
			return 3;
		}
	}
	n = atoi(argv[2]);
	root = POBJ_ROOT(pop, struct my_root);
	i = 0;

	// Create root if not already allocated
	if (TOID_IS_NULL(D_RO(root)->rect)) {
		TX_BEGIN(pop) {
			TX_ADD(root);
			POBJ_NEW(pop, &D_RW(root)->rect, struct rectangle, rect_construct, NULL);
		} TX_ONCOMMIT {
			printf("Created root\n");
			i++;
		} TX_END
	} else {
		printf("ROOT exists\n");
	}

	printf("Creating %d more object(s)\n", n);

	// we are not keeping track of objects ourselves
	while (i < n) {
		TX_BEGIN(pop) {	
			rect = D_RO(root)->rect;
			TX_ADD(rect);
			POBJ_NEW(pop, &rect, struct rectangle, rect_construct, NULL);
		} TX_ONCOMMIT {
			printf("Created objects #%d\n", i);
		} TX_END
		i++;
	}

	// Deallocate rect in the reader
	pmemobj_close(pop);
	return 0;
}

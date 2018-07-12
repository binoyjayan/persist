#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	int p;
	struct stat s;
	PMEMobjpool *pop;
	TOID(struct my_root) root;
	TOID(struct rectangle) rect;

	if (argc < 2) {
		printf("Usage: %s <pmem file>\n", argv[0]);
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

	root = POBJ_ROOT(pop, struct my_root);

	TX_BEGIN(pop) {
		TX_ADD(root);
		if (TOID_IS_NULL(D_RO(root)->rect)) {
			rect = TX_NEW(struct rectangle);
		} else {
			pmemobj_tx_abort(-1);
		}
		D_RW(rect)->a = 5;
		D_RW(rect)->b = 10;
		D_RW(root)->rect = rect;
	} TX_ONCOMMIT {
		printf("Allocated rect\n");
	} TX_ONABORT {
		printf("Aborted transaction. rect already allocated\n");
	} TX_END

	// Deallocate rect in the reader
	pmemobj_close(pop);
	return 0;
}

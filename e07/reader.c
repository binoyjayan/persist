#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	int p, n;
	PMEMobjpool *pop;
	TOID(struct my_root) root;
	TOID(struct rectangle) iter, rect;

	if (argc < 2) {
		printf("Usage: %s <pmem file>\n", argv[0]);
		return 1;
	}

	pop = pmemobj_open(argv[1], POBJ_LAYOUT_NAME(rect_calc));
	if (pop == NULL) {
		perror("pmemobj_open");
		return 2;
	}

	root = POBJ_ROOT(pop, struct my_root);

	// Iterating through objects using internal collections
	// without using referring to root
	n = 0;
	POBJ_FOREACH_TYPE(pop, iter) {
		p = perimeter_calc(iter);
		printf("rectangle %d: a:%d b:%d perimeter:%d\n", n, D_RO(iter)->a, D_RO(iter)->b, p);
		n++;
	}

	printf("Found %d objects\n" ,n);
	if (!n)
		goto close_obj;

	n = 0;
	// Deallocate objects
	iter = POBJ_FIRST(pop, struct rectangle);
	while(!TOID_IS_NULL(iter)) {
		rect = iter;
		iter = POBJ_NEXT(iter);
		TX_BEGIN(pop) {
			TX_ADD(rect);
			POBJ_FREE(&rect);
		} TX_ONCOMMIT {
			printf("Free object %d\n", n);
			n++;
		} TX_END
	}
	printf("Freed %d objects\n", n);

	TX_BEGIN(pop) {
		TX_ADD(root);
		POBJ_FREE(&D_RW(root)->rect);
	} TX_ONCOMMIT {
		printf("reste head object\n");
	} TX_END

close_obj:
	pmemobj_close(pop);
	return 0;
}



#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	int p;
	PMEMobjpool *pop;
	TOID(struct my_root) root;
	TOID(struct rectangle) rect;

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
	rect = D_RO(root)->rect;

	if (TOID_IS_NULL(rect)) {
		printf("rect is NULL. Run writer to allocate rect\n");
		goto close_obj;
	}
	p = perimeter_calc(rect);
	printf("a:%d b:%d perimeter:%d\n", D_RO(rect)->a, D_RO(rect)->b, p);

	// Deallocate in the reader here
	TX_BEGIN(pop) {
		TX_ADD(root);
		POBJ_FREE(&D_RW(root)->rect); // rect is auto zeroed after free
	} TX_ONCOMMIT {
		printf("Freed rect\n");
	} TX_END

close_obj:
	pmemobj_close(pop);
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	PMEMoid root;
	struct vector *vp;

	if (argc < 2) {
		printf("Usage: %s <pmem file>\n", argv[0]);
		return 1;
	}

	PMEMobjpool *pop = pmemobj_open(argv[1], LAYOUT_NAME);
	if (pop == NULL) {
		perror("pmemobj_open");
		return 2;
	}

	root = pmemobj_root(pop, sizeof (struct vector));
	vp = pmemobj_direct(root);

	printf("x:%d y:%d z:%d\n", vp->x, vp->y, vp->z);

	pmemobj_close(pop);
	return 0;
}

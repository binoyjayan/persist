#include <stdio.h>
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
	struct my_root *rootp;

	if (argc < 3) {
		printf("Usage: %s <pmem file> <string to write>\n", argv[0]);
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

	root = pmemobj_root(pop, sizeof (struct my_root));
	rootp = pmemobj_direct(root);

	rootp->len = strlen(argv[2]);
	pmemobj_persist(pop, &rootp->len, sizeof (rootp->len));
	pmemobj_memcpy_persist(pop, rootp->buf, argv[2], rootp->len + 1);

	pmemobj_close(pop);
	return 0;
}

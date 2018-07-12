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
	TOID(struct my_root) root;
	PMEMobjpool *pop;
	struct my_root *rootp;

	if (argc < 3) {
		printf("Usage: %s <pmem file> <string to write>\n", argv[0]);
		return 1;
	}

	if (stat(argv[1], &s) < 0) {
		pop = pmemobj_create(argv[1], POBJ_LAYOUT_NAME(string_store), PMEMOBJ_MIN_POOL, 0666);
		if (pop == NULL) {
			perror("pmemobj_create");
			return 2;
		}
	} else {
		pop = pmemobj_open(argv[1], POBJ_LAYOUT_NAME(string_store));
		if (pop == NULL) {
			perror("pmemobj_create");
			return 3;
		}
	}

	root = POBJ_ROOT(pop, struct my_root);
	TX_BEGIN(pop) {
		D_RW(root)->len = strlen(argv[2]);
		TX_MEMCPY(D_RW(root)->buf, argv[2], strlen(argv[2]) + 1);
	} TX_END
	pmemobj_close(pop);
	return 0;
}

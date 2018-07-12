#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libpmemobj.h>
#include "layout.h"

void do_work(PMEMobjpool *pop, char *argv[])
{
	PMEMoid root;
	struct pair *vp;

	struct pair *v = malloc(sizeof(struct pair));
	if (v == NULL)
		return;

	root = pmemobj_root(pop, sizeof (struct pair));
	vp = pmemobj_direct(root);

	printf("Allocate pair %p\n", v);
	TX_BEGIN(pop) {
		pmemobj_tx_add_range(root, sizeof(int), sizeof(struct pair));
		v->y = atoi(argv[3]) * 2;
		vp->y = v->y;
		printf("Inner transaction x:%d y:%d\n", vp->x, vp->y);
	} TX_FINALLY {
		printf("Free pair %p\n", v);
		free(v);
	} TX_END
}

int main(int argc, char *argv[])
{
	struct stat s;
	PMEMoid root;
	PMEMobjpool *pop;
	struct pair *vp;

	if (argc < 4) {
		printf("Usage: %s <pmem file> <2 numbers>\n", argv[0]);
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

	root = pmemobj_root(pop, sizeof (struct pair));
	vp = pmemobj_direct(root);

	// Write initial values for x and y
	TX_BEGIN(pop) {
		pmemobj_tx_add_range(root, 0, sizeof(struct pair));
		vp->x = atoi(argv[2]);
		vp->y = atoi(argv[3]);
	} TX_END

	// perform outer transaction add 1000 to x
	TX_BEGIN(pop) {
		pmemobj_tx_add_range(root, 0, sizeof(int));
		vp->x += 1000;
		printf("Outer transaction x:%d y:%d\n", vp->x, vp->y);
		do_work(pop, argv);
		pmemobj_tx_abort(-1);
	} TX_END
	pmemobj_close(pop);
	return 0;
}



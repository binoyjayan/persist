#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	char buf[MAX_BUF_LEN];
	PMEMoid root;
	struct my_root *rootp;
	int n;

	if (argc < 2) {
		printf("Usage: %s <pmem file>\n", argv[0]);
		return 1;
	}

	PMEMobjpool *pop = pmemobj_open(argv[1], LAYOUT_NAME);
	if (pop == NULL) {
		perror("pmemobj_open");
		return 2;
	}

	root = pmemobj_root(pop, sizeof (struct my_root));
	rootp = pmemobj_direct(root);

	n = strlen(rootp->buf);
	printf("rootlen:%d buflen:%d\n", (int) rootp->len, n);

	if (n && rootp->len == n)
		printf("SAME LENGTH %s\n", rootp->buf);
	else
		printf("DIFF LENGTH %s\n", rootp->buf);

	pmemobj_close(pop);
	return 0;
}

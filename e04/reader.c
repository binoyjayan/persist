#include <stdio.h>
#include <string.h>
#include <libpmemobj.h>
#include "layout.h"

int main(int argc, char *argv[])
{
	char buf[MAX_BUF_LEN];
	TOID(struct my_root) root;
	int n1, n2;

	if (argc < 2) {
		printf("Usage: %s <pmem file>\n", argv[0]);
		return 1;
	}

	PMEMobjpool *pop = pmemobj_open(argv[1], POBJ_LAYOUT_NAME(string_store));
	if (pop == NULL) {
		perror("pmemobj_open");
		return 2;
	}

	root = POBJ_ROOT(pop, struct my_root);

	n1 = D_RO(root)->len;
	n2 = strlen(D_RO(root)->buf);
	printf("rootlen:%d buflen:%d\n", n1, n2);

	if (n1 && n1 == n2)
		printf("SAME LENGTH: %s\n", D_RO(root)->buf);
	else
		printf("DIFF LENGTH: %s\n", D_RO(root)->buf);

	pmemobj_close(pop);
	return 0;
}


POBJ_LAYOUT_BEGIN(rect_calc);
	POBJ_LAYOUT_ROOT(rect_calc, struct my_root);
	POBJ_LAYOUT_TOID(rect_calc, struct rectangle);
POBJ_LAYOUT_END(rect_calc);

struct rectangle {
	int a;
	int b;
};

int rect_construct(PMEMobjpool *pop, void *ptr, void *arg) {
	struct rectangle *rect = ptr;
	rect->a = 5;
	rect->b = 10;
	pmemobj_persist(pop, rect, sizeof *rect);
	return 0;
}

struct my_root {
	TOID(struct rectangle) rect;
};

static inline int area_calc(const TOID(struct rectangle) rect)
{
	return D_RO(rect)->a * D_RO(rect)->b;
}

static inline int perimeter_calc(const TOID(struct rectangle) rect)
{
	return (D_RO(rect)->a + D_RO(rect)->b) * 2;
}

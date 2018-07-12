all: writer reader

writer: writer.c
	gcc -o writer writer.c -lpmemobj

reader: reader.c
	gcc -o reader reader.c -lpmemobj

clean:
	rm reader writer mydev
run:
	PMEM_IS_PMEM_FORCE=1 ./writer mydev "Hello World"
	PMEM_IS_PMEM_FORCE=1 ./reader mydev

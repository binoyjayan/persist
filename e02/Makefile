all: writer reader run

writer: writer.c
	gcc -o writer writer.c -lpmemobj

reader: reader.c
	gcc -o reader reader.c -lpmemobj

clean:
	rm reader writer mydev
run: writer reader
	PMEM_IS_PMEM_FORCE=1 ./writer mydev 10 20 30
	PMEM_IS_PMEM_FORCE=1 ./reader mydev

.PHONY: run clean

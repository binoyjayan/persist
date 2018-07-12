all: writer reader run

writer: writer.c
	gcc -o writer writer.c -lpmemobj

reader: reader.c
	gcc -o reader reader.c -lpmemobj

clean:
	rm reader writer /tmp/mydev

run: writer reader
	PMEM_IS_PMEM_FORCE=1 ./writer /tmp/mydev "Hello World"
	PMEM_IS_PMEM_FORCE=1 ./reader /tmp/mydev

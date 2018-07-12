all: writer reader

writer: writer.c layout.h
	gcc -o writer writer.c -lpmemobj

reader: reader.c layout.h
	gcc -o reader reader.c -lpmemobj

clean:
	rm reader writer /tmp/mydev

run: writer reader
	PMEM_IS_PMEM_FORCE=1 ./writer /tmp/mydev
	PMEM_IS_PMEM_FORCE=1 ./reader /tmp/mydev

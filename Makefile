all: writer reader run

writer: writer.c
	gcc -o writer writer.c

reader: reader.c
	gcc -o reader reader.c

run: writer reader
	./writer
	./reader

clean:
	rm -f writer reader shm_key.txt
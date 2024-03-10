CC = gcc
CFLAGS = -Wall -g

build: tema3

tema3: graf-L.o main.o lista.o heap.o
	$(CC) $(CFLAGS) $^ -o $@

graf-L.o: graf-L.c graf-L.h

main.o: main.c graf-L.h

heap.o: heap.c heap.h

lista.o: lista.c lista.h

run:
	./graf

.PHONY: clean
clean:
	rm -f *.o *~ graf

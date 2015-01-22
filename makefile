CC = gcc
CFLAGS = -m32
DEPS = primes.h
OBJ = test_malloc.o 

malloc.o: malloc.c
	gcc -m32 -c malloc.c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

test_malloc: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ malloc.c








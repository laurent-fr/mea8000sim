CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = main.o bin.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mea8000sim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
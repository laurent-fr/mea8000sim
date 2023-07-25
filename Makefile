CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
DEPS = bin.h filter.h synth.h
OBJ = main.o bin.o filter.o synth.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mea8000sim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
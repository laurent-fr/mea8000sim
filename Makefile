CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm -lsndfile
DEPS = bin.h filter.h synth.h config.h
OBJ = main.o bin.o filter.o synth.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mea8000sim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o 
	rm -f mea8000sim 
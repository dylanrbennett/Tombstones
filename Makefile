CC=g++
CFLAGS=-g
DEPS = 
OBJ = main.o Tombstone.o
TARGET=driver

test: $(OBJ) $(TARGET)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ $(TARGET)

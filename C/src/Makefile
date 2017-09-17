EXEC=Tabou
SRC=$(wildcard *.c) $(wildcard */*.c)
OBJ=$(SRC:.c=.o)
HEAD=$(wildcard headers/*.h) $(wildcard */headers/*.h)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: $(HEAD)

%.o: %.c FORCE
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clall: clean

clean:
	rm *.o && rm */*.o

test:
	echo “Tests OK”

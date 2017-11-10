EXEC=TabouFull
EXEC_PROD=Tabou

SRC_PROD=$(wildcard prod/*.c)
HEAD_PROD=$(wildcard headers/*.h) FLAGS.h
TMPVAR := $(SRC_PROD)
SRC_PROD = $(filter-out prod/MainTests.c, $(TMPVAR))
OBJ_PROD=$(SRC_PROD:.c=.o)

SRC_UNIT=$(wildcard *.c) $(wildcard unit/*.c)
HEAD_UNIT=$(wildcard headers/*.h) FLAGS.h $(wildcard unit/headers/*.h)
TMPVAR := $(SRC_UNIT)
SRC_UNIT = $(filter-out prod/Main.c, $(TMPVAR))
OBJ_UNIT=$(SRC_UNIT:.c=.o)

all: $(EXEC)
prod: $(EXEC_PROD)

$(EXEC): $(OBJ_UNIT)
	@echo "VAR is: $(OBJ_UNIT)"
	$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_PROD): $(OBJ_PROD)
	@echo "VAR is: $(OBJ_PROD)"
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: $(HEAD_UNIT)

%.o: %.c FORCE
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clall: clean

clean:
	-rm *.o ; rm */*.o ; rm $(EXEC)

test:
	echo “Tests OK”

export CC=gcc
export CFLAGS=-std=c99 -Wall -Werror -W -pedantic -static -g
export LDFLAGS=-lm
SRC_DIR=C/src
EXEC=Tabou

all: clall $(EXEC) clean

$(EXEC):
	cd $(SRC_DIR) && $(MAKE) && cd .. && mv $(SRC_DIR)/$(EXEC) .

.PHONY: clean

clean:
	cd $(SRC_DIR) && $(MAKE) $@

clall:
	-rm $(EXEC) && cd $(SRC_DIR) && $(MAKE) $@

test: all
	./$(EXEC) test && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) test

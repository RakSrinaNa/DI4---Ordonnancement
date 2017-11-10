export CC=gcc
export CFLAGS=-std=c99 -Wall -Werror -W -pedantic -pedantic-errors -static -g
export LDFLAGS=-lm
SRC_DIR=C/src

EXEC=TabouFull
EXEC_PROD=Tabou

all: clall $(EXEC) clean
prod: clall $(EXEC_PROD) clean

$(EXEC):
	cd $(SRC_DIR) && $(MAKE) && cd ../.. && mv $(SRC_DIR)/$(EXEC) ./C

$(EXEC_PROD):
	cd $(SRC_DIR) && $(MAKE) && cd ../.. && mv $(SRC_DIR)/$(EXEC_PROD) ./C

.PHONY: clean

clean:
	cd $(SRC_DIR) && $(MAKE) $@

clall:
	-rm $(EXEC) ; rm $(EXEC_PROD) ; cd $(SRC_DIR) && $(MAKE) $@

test: all
	cd C && ./$(EXEC) test && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) test

export CC=gcc
export LDFLAGS=-lm
SRC_DIR=C/src

EXEC=TabouFull
EXEC_PROD=Tabou

all: clall $(EXEC) clean $(EXEC_PROD) clean
test: clall $(EXEC) clean
prod: clall $(EXEC_PROD) clean

$(EXEC):
	export CFLAGS='-std=c99 -Wall -Wextra -pedantic -g'
	cd $(SRC_DIR) && $(MAKE) && cd ../.. && mv $(SRC_DIR)/$(EXEC) ./C

$(EXEC_PROD):
	export CFLAGS='-std=c99 -Wall -Wextra -Werror -W -pedantic -pedantic-errors -O2'
	cd $(SRC_DIR) && $(MAKE) prod && cd ../.. && mv $(SRC_DIR)/$(EXEC_PROD) ./C

.PHONY: clean

clean:
	cd $(SRC_DIR) && $(MAKE) $@

clall:
	-rm $(EXEC) ; rm $(EXEC_PROD) ; cd $(SRC_DIR) && $(MAKE) $@

doTest: all
	cd C && ./$(EXEC) test && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) test && ./$(EXEC_PROD) unitResources/Instance1.txt && cd ..

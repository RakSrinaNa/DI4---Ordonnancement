export CC=gcc
export LDFLAGS=-lm
SRC_DIR=C

EXEC=TabouTest
EXEC_PROD=TabouProd

all: $(EXEC) $(EXEC_PROD)
test: $(EXEC)
prod: $(EXEC_PROD)

$(EXEC):
	cd $(SRC_DIR) && cmake . && make TabouProd && cd ..

$(EXEC_PROD):
	cd $(SRC_DIR) && cmake . && make TabouTest && cd ..

.PHONY: clean

doTest: all
	cd C && ./$(EXEC) --test && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) --test && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) ../Inputs/I_1_5_20_2.txt && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC_PROD) ../Inputs/I_1_5_20_2.txt && cd ..

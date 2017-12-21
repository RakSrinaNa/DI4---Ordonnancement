export CC=gcc
export LDFLAGS=-lm
SRC_DIR=C

EXEC=TabouTest
EXEC_PROD=TabouProd

all: $(EXEC) $(EXEC_PROD)
test: $(EXEC)
prod: $(EXEC_PROD)

$(EXEC):
	cd $(SRC_DIR) && cmake . && make TabouTest && cd ..

$(EXEC_PROD):
	cd $(SRC_DIR) && cmake . && make TabouProd && cd ..

.PHONY: all

doTestUnit: $(EXEC)
	cd C && ./$(EXEC) --test && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) --test && cd ..

doTestValgrind: all
	cd C && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC) ../Inputs/I_1_5_20_2.txt && valgrind --track-origins=yes --leak-check=full --error-exitcode=50 ./$(EXEC_PROD) ../Inputs/I_1_5_20_2.txt && cd ..

doTestSuite: $(EXEC_PROD)
	java -jar ./Java/SolutionCalculator/solutionCalculator.jar --c "./C/$(EXEC_PROD)" --p "./Python/tabu_W_v3.py" --i "./Inputs/I_1_5_20_*.txt" --i "./Inputs/I_1_5_50_*.txt" --i "./Inputs/I_1_5_100_*.txt"

doTest: all doTestUnit doTestValgrind doTestSuite
	 

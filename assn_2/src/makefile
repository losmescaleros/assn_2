# CSC 541 Assignment 2
# Mitchell Neville
#
# This makefile will compile an in-memory indexiing program that 
# explores various strategies for availability lists. It will create
# a database called 'student.db' for persisting student records to disk.
# Object files are placed into a folder called 'obj'. 
#
# Usage:
# $ make			Compiles the program
# $ make clean		Removes any test output files and intermediate compilation files
# $ make test		Runs separate test cases from ../data/examples/ and outputs
#					diff statement results from these tests to 'test_results.txt'.
#					Output files for 'best-fit', 'first-fit', and 'worst-fit' options
#					are created. 

# Source, data, and include folders
SRCDIR = ../src
IDIR = ../include
ODIR = obj
LDIR = ../lib

# Compile parameters
CC = g++
CFLAGS = -Wall -I $(IDIR)
_DEPS = main.h
DEPS = $(patsubst %, $(IDIR)/$, $(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))
EXEC = assn_2

TEST_RESULTS = test_results.txt
EXDIR = ../data/examples
INPUT1 = input-01.txt
INPUT2 = input-02.txt

OUTPUT1_FIRST = output-01-first.txt
OUTPUT2_FIRST = output-02-first.txt
OUTPUT1_BEST = output-01-best.txt
OUTPUT2_BEST = output-02-best.txt

MY_OUTPUT1_FIRST = my-output-01-first.txt
MY_OUTPUT2_FIRST = my-output-02-first.txt
MY_OUTPUT1_BEST = my-output-01-best.txt
MY_OUTPUT2_BEST = my-output-02-best.txt
MY_OUTPUT1_WORST = my-output-01-worst.txt
MY_OUTPUT2_WORST = my-output-02-worst.txt

$(ODIR)/%.o: %.cpp $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

assn_2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: assn_2

clean: clean_tests
	rm -rf $(ODIR) *.bin *.db

clean_tests:
	rm -f $(TEST_RESULTS) $(MY_OUTPUT1_FIRST) $(MY_OUTPUT2_FIRST) $(MY_OUTPUT1_BEST) $(MY_OUTPUT2_BEST) $(MY_OUTPUT1_WORST) $(MY_OUTPUT2_WORST)
	
test: assn_2 clean_tests clean
	### First-fit Tests
	./$(EXEC) --first-fit student.db < $(EXDIR)/$(INPUT1) > $(MY_OUTPUT1_FIRST) 
	echo "--- First-Fit Test ---" >> $(TEST_RESULTS)
	echo "--- DIFF OF OUTPUT BASED ON INPUT 01:" >> $(TEST_RESULTS)
	-diff $(MY_OUTPUT1_FIRST) $(EXDIR)/$(OUTPUT1_FIRST) >> $(TEST_RESULTS)
	./$(EXEC) --first-fit student.db < $(EXDIR)/$(INPUT2) > $(MY_OUTPUT2_FIRST)
	echo "--- DIFF OF OUPTUT BASED ON INPUT 02:" >> $(TEST_RESULTS)
	-diff $(MY_OUTPUT2_FIRST) $(EXDIR)/$(OUTPUT2_FIRST) >> $(TEST_RESULTS)
	### Best-fit Tests
	rm -f *.bin *.db
	./$(EXEC) --best-fit student.db < $(EXDIR)/$(INPUT1) > $(MY_OUTPUT1_BEST) 
	echo " " >> $(TEST_RESULTS)
	echo "--- Best-Fit Test ---" >> $(TEST_RESULTS)
	echo "--- DIFF OF OUTPUT BASED ON INPUT 01:" >> $(TEST_RESULTS)
	-diff $(MY_OUTPUT1_BEST) $(EXDIR)/$(OUTPUT1_BEST) >> $(TEST_RESULTS)
	./$(EXEC) --first-fit student.db < $(EXDIR)/$(INPUT2) > $(MY_OUTPUT2_BEST)
	echo "--- DIFF OF OUPTUT BASED ON INPUT 02:" >> $(TEST_RESULTS)
	-diff $(MY_OUTPUT2_BEST) $(EXDIR)/$(OUTPUT2_BEST) >> $(TEST_RESULTS)
	### Worst-fit Tests
	rm -f *.bin *.db
	./$(EXEC) --worst-fit student.db < $(EXDIR)/$(INPUT1) > $(MY_OUTPUT1_WORST) 
	echo " " >> $(TEST_RESULTS)
	echo "--- Worst-Fit Test ---" >> $(TEST_RESULTS)
	echo "--- DIFF OF OUTPUT BASED ON INPUT 01:" >> $(TEST_RESULTS)
	-diff $(MY_OUTPUT1_WORST) $(EXDIR)/$(OUTPUT1_WORST) >> $(TEST_RESULTS)
	./$(EXEC) --worst-fit student.db < $(EXDIR)/$(INPUT2) > $(MY_OUTPUT2_WORST)
	echo "--- DIFF OF OUPTUT BASED ON INPUT 02:" >> $(TEST_RESULTS)
	-diff $(MY_OUTPUT2_WORST) $(EXDIR)/$(OUTPUT2_WORST) >> $(TEST_RESULTS)
	

	


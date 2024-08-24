# Compiler
CXX = g++

# Executable name
CXXFLAGS = -std=c++11 -Wall -Wextra

.PHONY: clean

main: main.o tsp_genetic_algorithm.o sort.o
	$(CXX) $(CXXFLAGS) main.o tsp_genetic_algorithm.o sort.o -o main

main.o: main.cpp tsp_genetic_algorithm.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tsp_genetic_algorithm.o: tsp_genetic_algorithm.cpp tsp_genetic_algorithm.hpp sort.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

sort.o: sort.cpp sort.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o main

main-exist:
	@if [ $(FILE_EXISTS) -eq 0 ]; then \
		echo "File main does not exist. Compile it first!"; \
		exit 1; \
	fi

test-run: main-exist
	@for param in 5, 6, 7; do \
		./main cities.csv $$param; \
	done
	@echo "PASSED"

FILE_EXISTS := $(shell if [ -e "main" ]; then echo 1; else echo 0; fi)
STRING_TO_MATCH = "112.4"
TEST_RUNS = 3
test-result: main-exist
	@correct=0; \
	for i in $$(seq 1 $(TEST_RUNS)); do \
		output=$$(./main cities.csv 7); \
		echo "$$output"; \
		if echo "$$output" | grep -q "$(STRING_TO_MATCH)"; then \
			echo "Test $$i: PASSED"; \
			correct=1; \
			break; \
		else \
			echo "Test $$i: FAILED"; \
		fi; \
	done; \

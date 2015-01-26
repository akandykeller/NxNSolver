###############################################################################
# NxNSolver makefile
###############################################################################
CC = g++
FLAGS = -g -o
SOURCES = *.cpp *.h

EXENAME = NxNSolver

all: $(SOURCES)
	$(CC) $(FLAGS) $(EXENAME) $(INCLUDE) $(SOURCES)

clean:
	rm -f *.o $(EXENAME)

.PHONY: all clean
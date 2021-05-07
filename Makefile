###################################
# OS COURSE - CS433 example
#
###################################
CC = g++                                                        # use g++ for compiling c++ or Gcc for c code
CFLAGS = -g -Wall -std=c++0x            # compilation flags: -g for debugging. Change to -O2 or -O3 for optimized code
LIB = -lm -lpthread                                     #linked libraries
LDFLAGS = -L.                                           #linked flags
PROG = prog4                                            #target executable (output)
SRCS = hw4.cpp                         #.c or .cpp source files.
OBJ = $(SRCS:.cpp=.o)           #object files for the target. Add more to this and next lines if theere are more than one source files.

all : $(PROG) depend

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS) $(LIB)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

# cleanup
clean:
	/bin/rm -f *.o $(PROG) .depend

#DO NOT DELETE

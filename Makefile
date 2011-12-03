CC := icc
CFLAGS := -Wall -g -O0 -DNDEBUG
LDFLAGS := -lrt

ifeq ($(DEBUG),1)
	CFLAG := $(CFLAG) -DDEBUG -O0
else
	CFLAG := $(CFLAG) -DNDEBUG -O3
endif

SRCS := main.cpp graph.cpp
OBJS := $(SRCS: .cpp=.o)

all: min_cost_flow

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
	
graph.o: graph.h
	$(CC) $(CFLAGS) -c graph.h

min_cost_flow: main.o graph.o
	$(CC) -o $(all) main.o graph.o $(LDFLAGS)

clean:
	rm -r min_cost_flow *.o

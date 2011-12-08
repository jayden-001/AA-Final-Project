CC = icc
CFLAGS = -Wall
LDFLAGS = -lrt

ifeq ($(DEBUG),1)
	CFLAGS = $(CFLAG) -O0 -DDEBUG
else
	CFLAGS = $(CFLAG) -g -O3 -DNDEBUG
endif

ODIR = obj
SRCS = main.cpp
HEADERS = graph.h vertex.h edge.h
_OBJ = $(SRCS:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))
ALL = min_cost_flow

all: $(ALL)

obj/%.o: %.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

min_cost_flow: $(OBJS) $(HEADERS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(ALL) $(OBJS)

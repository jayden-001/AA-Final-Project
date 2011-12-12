CC = icc
CFLAGS = -Wall
LDFLAGS = -lrt

ifeq ($(DEBUG),1)
	CFLAGS := $(CFLAGS) -g -O0 -DDEBUG
else
	CFLAGS := $(CFLAGS) -O3 -DNDEBUG
endif

ODIR = obj
SRCS = main.cpp \
sequential_maxflow.cpp \
discharge.cpp parallel_maxflow.cpp reducer_list.cpp \
edmond-karp.cpp \
relabel_to_front.cpp \
gap_heuristic.cpp
HEADERS = graph.h vertex.h edge.h
_OBJ = $(SRCS:.cpp=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))
ALL = min_cost_flow

all: $(ALL)

#obj/%.o: %.cpp $(HEADERS)
#	$(CC) -c -o $@ $< $(CFLAGS)

min_cost_flow: $(SRCS) $(HEADERS)
	$(CC) -o $@ main.cpp $(LDFLAGS) $(CFLAGS)

clean:
	rm -rf $(ALL) $(OBJS)

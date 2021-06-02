CC=g++
CFLAGS=-Wall -Wno-unused-parameter -Wextra -O2 -pedantic -std=c++14 -I$(IDIR)
ODIR=obj
IDIR=include
SDIR=src
BIN=TSP

_DEPS=solver.h
DEPS=$(addprefix $(IDIR)/,$(_DEPS))

_OBJS=main.o solver.o
OBJS=$(addprefix $(ODIR)/,$(_OBJS))

$(BIN): $(OBJS) $(IDIR)/types.h
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJS):$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	mkdir $@

.PHONY: clean

clean:
	rm -rf $(ODIR) $(BIN)
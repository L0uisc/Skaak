IDIR = inc
SDIR = src
CC = g++
CFLAGS = -I$(IDIR)

ODIR = obj

_DEPS = bord.h InputValidationRoutines.h konstantes.h skuif.h stukke.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o bord.o InputValidationRoutines.o skuif.o stukke.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

BINDIR = bin

skaak: $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(BINDIR)/*

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=../obj

_DEPS = parse.h fileutil.h tokenize.h linked_list.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = subsetsum.o parse.o fileutil.o tokenize.o linked_list.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

subsetsum: $(OBJ)
	$(CC) -o ../bin/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

# Source, data, and include folders
SRCDIR = ../src
IDIR = ../include
ODIR = ../lib

# Compile parameters
CC = g++
CFLAGS = -Wall
_DEPS = main.h
DEPS = $(patsubst %, $(IDIR)/$, $(_DEPS))

LIBS = -lm

_OBJ = main.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assn_2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: assn_2

clean:
	rm -f $(ODIR)/*.o


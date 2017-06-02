CC = gcc
CFLAGS = -g -Wall -std=gnu11
LFLAGS =
SRCDIR = src
BINDIR = bin
OBJDIR = obj
VPATH = $(SRCDIR)
ENTRYPOINT = $(SRCDIR)/main.c
OBJECTS = discovery.o mac_ip_pair.o
OBJECT_FILES = $(addprefix $(OBJDIR)/, $(OBJECTS))
BIN = main

all: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(ENTRYPOINT) -o $(BINDIR)/$(BIN) $(LFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $(SRCDIR)/$*.c -o $(OBJDIR)/$*.o $(LFLAGS)

.PHONY: clean
clean:
	rm -f $(BINDIR)/$(BIN)
	rm -f $(OBJDIR)/*.o

.PHONY: run
run:
	./$(BINDIR)/$(BIN)

.PHONY: debug
debug:
	gdb ./$(BINDIR)/$(BIN)

.PHONY: test
test:
	valgrind ./$(BINDIR)/$(BIN)

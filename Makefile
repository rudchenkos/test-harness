.POSIX:
.PHONY: clean

LIB=src/libtestharness.a
OBJS=src/context.o src/expect.o src/watch.o
CFLAGS+=-I include/

$(LIB): $(OBJS)
	ar -rs $@ $(OBJS)

TEST_CFLAGS=$(CFLAGS) -DTEST -L src/ -ltestharness

examples/parser.test: examples/parser.c $(LIB)
	$(CC) $(TEST_CFLAGS) -o$@ examples/parser.c

clean:
	rm -r $(LIB) $(OBJS) examples/*.test

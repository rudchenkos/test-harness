.POSIX:
.PHONY: clean

INCLUDES=include/test-harness.h
LIB=src/libtestharness.a
OBJS=src/test.o src/context.o src/expect.o src/report.o src/watch.o src/main.o
CFLAGS+=-I include/

$(LIB): $(OBJS) $(INCLUDES)
	ar -rs $@ $(OBJS)

TEST_CFLAGS=$(CFLAGS) -DTEST -L src/ -ltestharness

examples: examples/palindrome.test
examples/palindrome.test: examples/palindrome.c $(LIB)
	$(CC) $(TEST_CFLAGS) -o$@ examples/palindrome.c

clean:
	rm -rf $(LIB) $(OBJS) examples/*.test

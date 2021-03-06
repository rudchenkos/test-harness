#include <test-harness.h>
#include <string.h>

#define fail test_harness_fail

void expect(const char* tag, bool value) {
    if (!value) {
        fail("expected that %s", tag);
    }
}

void expect_int(const char* tag, int actual, int expected) {
    if (actual != expected) {
        fail("expected %s %d but got %d", tag, expected, actual);
    }
}

void expect_string(const char* tag, const char* actual, const char* expected) {
    if (expected == NULL && actual != NULL) {
        fail("expected %s to be NULL but got '%s'", tag, actual);
    }
    else if (expected != NULL && actual == NULL) {
        fail("expected %s '%s' but got NULL", tag, expected);
    }
    else if (actual != expected && strcmp(actual, expected) != 0) {
        fail("expected %s '%s' but got '%s'", tag, expected, actual);
    }
}

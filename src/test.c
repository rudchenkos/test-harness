#include <test-harness.h>
#include "internals.h"
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>

static jmp_buf __test_start = {0};

bool test_harness_begin_test(const char* name) {
    if (setjmp(__test_start) == 0) {
        test_harness_push_context(name);
        test_harness_report_test_start();
        return true; // Client should perform the test
    } else {
        // Returned from a test, with a failure
        return false;
    }
}

void test_harness_end_test() {
    test_harness_report_test_passed();
    test_harness_pop_context();
}

void test_harness_fail(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char* description = NULL;
    vasprintf(&description, fmt, ap);
    va_end(ap);
    test_harness_report_test_failed(description ? description : "UNKNOWN(not enough memory)");
    free(description);

    longjmp(__test_start, 1);
}

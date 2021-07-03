#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/errno.h>

static int __ran_tests;
static int __failed_tests;
static jmp_buf __test_start = {0};

/* Test context stack, for diagnostic messages */
#define __MAX_TEST_CONTEXTS 64
static size_t __ncontexts = 0;
static const char* __contexts[__MAX_TEST_CONTEXTS];

static void __push_test_context(const char* description) {
    if (__ncontexts < __MAX_TEST_CONTEXTS - 1) {
        __contexts[__ncontexts] = description;
        ++__ncontexts;
    }
}

static void __pop_test_context() {
    assert(__ncontexts > 0);
    --__ncontexts;
}

static void __trace_context(const char* fmt, ...) {
    static int disabled = -1;
    if (disabled < 0) {
        const char* var = getenv("TEST_NOTRACE");
        disabled = var != NULL ? atoi(var) : 0;
    }
    if (disabled) return;

    va_list ap;
    va_start(ap, fmt);
    printf("%*c", (int)__ncontexts  * 2, ' '); // Indentation
    vprintf(fmt, ap);
    va_end(ap);
}

#define __TEST_CONTEXT(description, block) \
    do { \
        __push_test_context(description); \
        block; \
        __pop_test_context(); \
    } while(false);

#define TEST_CONTEXT(description, block) \
    __trace_context("\e[0;32m%s\e[0m\n", description); \
    __TEST_CONTEXT(description, block);

#define TEST_IT(title, block) __TEST_CONTEXT(title, \
    do { \
        ++__ran_tests; \
        if (setjmp(__test_start) != 0) { \
            ++__failed_tests; \
        } \
        else { \
            block; \
            __trace_context("\e[0;32m%s\e[0m\n", title); \
        } \
		errno = 0; \
    } while (false);)

void test_harness_fail(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "\e[0;31mFAILED: %s", __contexts[0]);
    for (int i = 1; i < __ncontexts; ++i) {
        fprintf(stderr, ": %s", __contexts[i]);
    }
    fputs("\n\t", stderr);
    vfprintf(stderr, fmt, ap);
    fputs("\e[0m\n", stderr);
    va_end(ap);
    longjmp(__test_start, 1);
}

#define begin_testing() \
    printf("=== Testing %s\n", __FILE__);

static int finish_testing() {
    if (__failed_tests) {
        printf("=== %d of %d tests failed\n", __failed_tests, __ran_tests);
        return 1;
    } else {
        printf("=== %d tests passed\n", __ran_tests);
        return 0;
    }
}


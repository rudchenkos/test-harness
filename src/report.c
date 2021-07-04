#include "internals.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static size_t total_tests = 0;
static size_t tests_passed = 0;
static size_t tests_failed = 0;

static bool quiet = false;

static void report_printf(const char* fmt, ...) {
    if (quiet) return;

    va_list ap;
    va_start(ap, fmt);
    printf("%*c", (int)__ncontexts  * 2, ' '); // Indentation
    vprintf(fmt, ap);
    va_end(ap);
}

void test_harness_report_begin(const char* suitename) {
    const char* var = getenv("TEST_QUIET");
    if (var != NULL && (strcasecmp(var, "true") || atoi(var))) {
        quiet = true;
    }
    report_printf("=== Testing %s\n", __FILE__);
}

void test_harness_report_test_start() {
    ++total_tests;
}

void test_harness_report_test_passed() {
    ++tests_passed;
    assert(__ncontexts > 0);
    report_printf("\e[0;32m%s\e[0m\n", __contexts[__ncontexts - 1]);
}

void test_harness_report_test_failed(const char* description) {
    ++tests_failed;
    fprintf(stderr, "\e[0;31mFAILED: %s", __contexts[0]);
    for (int i = 1; i < __ncontexts; ++i) {
        fprintf(stderr, ": %s", __contexts[i]);
    }
    fputs("\n\t", stderr);
    fputs(description, stderr);
    fputs("\e[0m\n", stderr);
}

int test_harness_report_end() {
    if (tests_failed) {
        report_printf("=== %zu of %zu tests failed\n", tests_failed, total_tests);
    } else {
        report_printf("=== %zu tests passed\n", total_tests);
    }
    return tests_failed;
}


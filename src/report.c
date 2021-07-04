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

bool test_harness_report_quiet = false;

static const char* current_suite_name = NULL;
static const char* current_test_name = NULL;

static void report_printf(const char* fmt, ...) {
    if (test_harness_report_quiet) return;

    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

void test_harness_report_begin(const char* suitename) {
    report_printf("=== Testing %s\n", suitename);
    current_suite_name = suitename;
}

void test_harness_report_test_start(const char* testname) {
    ++total_tests;
    current_test_name = testname;
}

void test_harness_report_test_passed() {
    ++tests_passed;
    assert(current_test_name);
    report_printf("  \e[0;32m%s\e[0m\n", current_test_name);
}

void test_harness_report_test_failed(const char* description) {
    ++tests_failed;
    fprintf(stderr, "\e[0;31mFAILED: %s: %s", current_suite_name, current_test_name);
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


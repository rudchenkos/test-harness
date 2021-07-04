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

static void print_tap_diagnostic(FILE* f, const char* diagnostic) {
    const char* line = diagnostic;
    do {
        const char* eol = strchr(line, '\n');
        if (!eol) {
            fprintf(f, "  # %s\n", line);
            break;
        } else {
            fprintf(f, "  # %.*s\n", (int)(eol - line), line);
            line = eol + 1;
        }
    } while (line != NULL && line[0] != 0);
}

void test_harness_report_begin(const char* suitename) {
    report_printf("# Testing %s\n", suitename);
    current_suite_name = suitename;
}

void test_harness_report_test_start(const char* testname) {
    ++total_tests;
    current_test_name = testname;
}

void test_harness_report_test_passed() {
    ++tests_passed;
    assert(current_test_name);
    report_printf("\e[0;32mok %zu - %s\e[0m\n", total_tests, current_test_name);
}

void test_harness_report_test_failed(const char* description) {
    ++tests_failed;
    fprintf(stderr, "\e[0;31mnot ok %zu - %s\n", total_tests, current_test_name);
    print_tap_diagnostic(stderr, description);
    fputs("\e[0m\n", stderr);
}

int test_harness_report_end() {
    if (tests_failed) {
        report_printf("# %zu of %zu tests failed\n", tests_failed, total_tests);
    } else {
        report_printf("# %zu tests passed\n", total_tests);
    }
    return tests_failed;
}


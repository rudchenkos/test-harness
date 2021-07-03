#ifndef __TEST_HARNESS_H__
#define __TEST_HARNESS_H__

#include <stdbool.h>

/** Fail the current test case */
void test_harness_fail(const char* fmt, ...);

/** Check if value is true, fail the test otherwise */
void expect(const char* tag, bool value);

/** Check integer equality, report failure otherwise */
void expect_int(const char* tag, int actual, int expected);

/** Check string equality, report failure otherwise */
void expect_string(const char* tag, const char* actual, const char* expected);

/** Run the current test suite in the watch mode
 * Listens to changes in src, rebuilds and restarts argv0
 */
void test_harness_start_watch(const char* argv0, const char* src);
#endif

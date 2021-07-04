#ifndef __TEST_HARNESS_INTERNALS_H__
#define __TEST_HARNESS_INTERNALS_H__

#include <sys/types.h>

#define __MAX_TEST_CONTEXTS 64

/* Test context stack, for diagnostic messages */
extern const char* __contexts[__MAX_TEST_CONTEXTS];
/* Current number of contexts in the stack */
extern size_t __ncontexts;

void test_harness_report_begin(const char* suitename);
void test_harness_report_test_start();
void test_harness_report_test_passed();
void test_harness_report_test_failed(const char* description);
int test_harness_report_end(); /** returns number of failed tests */

/** Run the current test suite in the watch mode
 * Listens to changes in src, rebuilds and restarts argv0
 */
void test_harness_start_watch(const char* argv0, const char* src);

#endif
